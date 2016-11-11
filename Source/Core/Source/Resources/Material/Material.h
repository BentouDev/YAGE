//
// Created by mrjaqbq on 13.05.16.
//

#ifndef GAME_MATERIAL_H
#define GAME_MATERIAL_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <Utils/List.h>
#include <Utils/String.h>
#include <Utils/Color.hpp>
#include <Utils/DefaultTrait.h>
#include <Utils/LinearAllocator.h>
#include "../../Gfx/OpenGl/OpenGLBase.h"
#include "../Resource.h"

namespace Gfx
{
	void setUniform(GLint location, float value);
	void setUniform(GLint location, int value);
	void setUniform(GLint location, glm::vec2 value);
	void setUniform(GLint location, glm::vec3 value);
	void setUniform(GLint location, glm::vec4 value);
	void setUniform(GLint location, glm::mat4x4 value);
	void setUniform(GLint location, Utils::Color value);

	class ShaderProgram;
}

namespace Resources
{
	class ShaderManager;
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
		uint32_t _offset;
		GLint	 _location;

	public:
		explicit IAutoUniform(const char*, const Core::Material&, const Resources::ShaderManager&);

		virtual void set() = 0;
	};

	template <typename T>
	class AutoUniform : public IAutoUniform
	{
	protected:
		T _storedValue;

	public:
		inline explicit AutoUniform(const char* name, const Core::Material& material, const Resources::ShaderManager& manager)
			: IAutoUniform(name, material, manager)
		{ }

		void set() override
		{
			Gfx::setUniform(_location, _storedValue);
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

	DECL_RESOURCE(Material)
	{
	protected:
		// LOAD MAX BUFFER COUNT AND MAX BUFFER SIZE FROM DRIVER

		// Shader only has program ID
		// this is where uniform buffers are stored

		Memory::IMemoryBlock& _memory;

		// for uniform blocks
		Memory::LinearAllocator* _allocator;

		Utils::List<IAutoUniform*> _uniforms;

		Utils::Handle<Gfx::ShaderProgram> _shader;

	//	std::map<std::string, uint32_t > uniformMap;
	//	std::vector<Uniform> uniforms;

		// GLOBAL - data that is shared between objects
		// LOCAL - array with data per object, limited size ofc

	public:
		inline explicit Material(Memory::IMemoryBlock& memory)
			: _memory(memory), _allocator(nullptr), _uniforms(_memory)
		{ }

		Material(const Material&) = delete;
		Material& operator=(const Material&) = delete;
		Material& operator=(Material&&) = delete;

		Material(Material&& other)
				: _memory(other._memory),
				  _allocator(other._allocator),
				  _uniforms(std::move(other._uniforms)),
				  _shader(std::move(other._shader))
		{
			other._allocator = nullptr;
			other._shader = Utils::Handle<Gfx::ShaderProgram>::invalid();
		}

		virtual ~Material()
		{
			for(IAutoUniform* uniform : _uniforms)
			{
				Memory::Delete(_memory, uniform);
			}

			_uniforms.clear();
		}

		template <typename T>
		Core::Material& addUniform(const char* name, const Resources::ShaderManager& manager)
		{
			IAutoUniform* uniform = YAGE_CREATE_NEW(_memory, AutoUniform<T>)(name, *this, manager);
			_uniforms.add(uniform);
			return *this;
		}

		void bindUniforms()
		{
			// TODO: uniform locations should be reloadeable
			for(IAutoUniform* uniform : _uniforms)
			{
				uniform->set();
			}
		}

		inline Core::Material& setShaderProgram(Utils::Handle<Gfx::ShaderProgram> shader)
		{ _shader = shader; return *this; }

		Utils::Handle<Gfx::ShaderProgram> getShaderProgram() const
		{ return _shader; }
	};

	class MaterialTrait : public Utils::DefaultTrait<Material> {};
}

#endif //GAME_MATERIAL_H
