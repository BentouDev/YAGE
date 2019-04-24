//
// Created by mrjaqbq on 13.05.16.
//

#ifndef GAME_MATERIAL_H
#define GAME_MATERIAL_H

#include <map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <Utils/List.h>
#include <Utils/String.h>
#include <Utils/Color.hpp>
#include <Utils/SmartHandle.h>
#include <Utils/LinearAllocator.h>
#include <Platform/Logger.h>
#include <Platform/Graphics/OpenGl/OpenGLBase.h>

#include "Core/Resources/Resource.h"

// temporary
#include "Core/Resources/Texture/Texture.h"

namespace Resources
{
    class Texture;
    class ShaderManager;
}

namespace Gfx
{
    class ShaderProgram;
}

namespace Memory
{
	class LinearAllocator;
}

namespace Meta
{
	class RegisterClass;
}

namespace Core
{
    // uniform - pass name, get location
    // set value to location

    // when updating value - update buffer directly
    // then, on draw, only bind that buffer
    // without setting everything once again

    class Material;

    struct Uniform
    {
        uint32_t location;
        type_t type;
    };

    class IAutoUniform
    {
    protected:
        uint32_t		_offset;
        GLint			_location;
        Core::Material&	_material;
        std::string     _debugName;

    public:
        explicit IAutoUniform(const char* name, Core::Material&, const Resources::ShaderManager&);
        virtual ~IAutoUniform() = default;

        virtual void set() = 0;
        const std::string& getName() const { return _debugName; }
    };

	YClass(Serialize())
    class Material : public IResource
    {
		friend class Meta::RegisterClass;

	protected:
        // LOAD MAX BUFFER COUNT AND MAX BUFFER SIZE FROM DRIVER

        // Shader only has program ID
        // this is where uniform buffers are stored

        std::map<std::string, uint32_t> _uniformMap;

        Memory::IMemoryBlock* _memory;

        // for uniform blocks
        Memory::LinearAllocator* _allocator;

        Utils::List<IAutoUniform*> _uniforms;

        Utils::Handle<Gfx::ShaderProgram> _shader;

    //	std::map<std::string, uint32_t > uniformMap;
    //	std::vector<Uniform> uniforms;

        // GLOBAL - data that is shared between objects
        // LOCAL - array with data per object, limited size ofc

        std::uint32_t _textureIndex;

    public:
		Material() : _memory{ nullptr }, _allocator{nullptr}
		{ }
        
		inline explicit Material(Memory::IMemoryBlock& memory)
            : _memory(&memory), _allocator(nullptr), _uniforms(), _textureIndex(0) // #NewAlloc
        { }

        Material(const Material&) = delete;
        Material& operator=(const Material&) = delete;
        Material& operator=(Material&&) = delete;

        Material(Material&& other)
            : IResource(std::move(other)),
              _memory(other._memory),
              _allocator(other._allocator),
              _uniforms(std::move(other._uniforms)),
              _shader(std::move(other._shader)),
              _textureIndex(0)
        {
            other._allocator = nullptr;
            other._shader    = Utils::Handle<Gfx::ShaderProgram>::invalid();
        }

        ~Material()
        {
            for (IAutoUniform* uniform : _uniforms)
            {
                Core::Logger::info("Delete uniform {}", uniform->getName());
                Memory::Delete(*_memory, uniform);
            }

            _uniforms.clear();
        }

        template <typename T>
        Core::Material& addUniform(const char* name, const Resources::ShaderManager& manager);

        template <typename T>
        void setUniform(const char* name, T value);

        void bindUniforms()
        {
            // TODO: uniform locations should be reloadeable
            _textureIndex = 0;
            for (IAutoUniform* uniform : _uniforms)
            {
                uniform->set();
                OpenGL::checkError();
            }
        }

        inline Core::Material& setShaderProgram(Utils::Handle<Gfx::ShaderProgram> shader)
        { _shader = shader; return *this; }

        Utils::Handle<Gfx::ShaderProgram> getShaderProgram() const
        { return _shader; }

        GLint getLocation(const char* name, const Resources::ShaderManager& manager) const;

        void setUniform(GLint location, float value);
        void setUniform(GLint location, int value);
        void setUniform(GLint location, glm::vec2 value);
        void setUniform(GLint location, glm::vec3 value);
        void setUniform(GLint location, glm::vec4 value);
        void setUniform(GLint location, glm::mat4x4 value);
        void setUniform(GLint location, Utils::Color value);
        void setUniform(GLint location, Utils::SmartHandle<Resources::Texture> texture);
    };

    template <typename T>
    class AutoUniform : public IAutoUniform
    {
    protected:
        T _storedValue;

    public:
        inline explicit AutoUniform(const char* name, Core::Material& material, const Resources::ShaderManager& manager)
            : IAutoUniform(name, material, manager)
        { }

        void setValue(T value)
        {
            _storedValue = value;
        }

        void set() override
        {
            _material.setUniform(_location, _storedValue);
        }

        AutoUniform& operator=(T value)
        {
            _storedValue = value;
            return *this;
        }

        operator T()
        {
            return _storedValue;
        }
    };

    template <typename T>
    Core::Material& Material::addUniform(const char* name, const Resources::ShaderManager& manager)
    {
        IAutoUniform* uniform = YAGE_CREATE_NEW((*_memory), AutoUniform<T>)(name, *this, manager);
        Core::Logger::info("Created uniform {}", name);
        _uniformMap.emplace(name, _uniforms.size());
        _uniforms.push_back(uniform);
        return *this;
    }

    template <typename T>
    void Material::setUniform(const char* name, T value)
    {
        auto itr = _uniformMap.find(name);
        if (itr != _uniformMap.end())
        {
            auto* prop     = _uniforms[itr->second];
            auto* autoProp = dynamic_cast<AutoUniform<T>*>(prop);

            YAGE_ASSERT (
                autoProp != nullptr,
                "Material : attemp to set uniform '{}' value of different type!",
                name
            );

            if (autoProp != nullptr)
            {
                autoProp->setValue(value);
            }
        }
    }
}

#endif //GAME_MATERIAL_H
