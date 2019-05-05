//
// Created by mrjaqbq on 21.05.16.
//

#ifndef GAME_SHADER_H
#define GAME_SHADER_H

#include <Utils/List.h>

#include <Platform/Graphics/OpenGl/OpenGLBase.h>
#include "Core/Resources/Resource.h"

namespace Resources
{
    class ShaderBuilder;
}

namespace Meta
{
	class RegisterClass;
}

namespace Gfx
{
    enum ShaderType : GLenum
    {
        VERTEX 	 = gl::VERTEX_SHADER,
        GEOMETRY = gl::GEOMETRY_SHADER,
        FRAGMENT = gl::FRAGMENT_SHADER
    };

    class Shader
    {
    friend class Resources::ShaderBuilder;

        ShaderType	_type;
        GLuint 		_handle;
        bool 		_isCompiled;

    public:
        explicit inline Shader(ShaderType type) : _type(type), _handle(0), _isCompiled(false)
        {
            _handle = gl::CreateShader(type);
        }

        ~Shader() noexcept
        {
            gl::DeleteShader(_handle);
        }

        Shader(const Shader& shader)
            : _type(shader._type), _handle(shader._handle), _isCompiled(shader._isCompiled)
        { }

        Shader(Shader&& shader)
            : _type(shader._type), _handle(shader._handle), _isCompiled(shader._isCompiled)
        { }

        Shader& operator= (const Shader& shader)
        {
            Shader tmp(shader);
            *this = std::move(tmp);
            return *this;
        }

        Shader& operator= (Shader&& other) noexcept
        {
            gl::DeleteShader(_handle);
            _handle = other._handle;
            _type = other._type;
            _isCompiled = other._isCompiled;
            other._handle = 0;
            return *this;
        }

        inline operator GLuint() const noexcept
        { return _handle; }

        inline bool isCompiled() const noexcept
        { return _isCompiled; }
    };

	YClass(Serialize())
    class ShaderProgram : public Core::IResource
    {
		friend class Meta::RegisterClass;

        GLuint _handle;

    public:
        ShaderProgram() : _handle(0) { }

        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ShaderProgram& operator=(ShaderProgram&&) = delete;

        ShaderProgram(ShaderProgram&& other) noexcept
            : IResource(std::move(other)),
             _handle(other._handle)
        {
            other._handle = 0;
        }

        ~ShaderProgram()
        {
            if (_handle > 0)
            {
                gl::DeleteProgram(_handle);
            }
        }

        void init(GLuint handle);

        inline operator GLuint() const noexcept
        { return _handle; }

        inline void bind() const noexcept;
    };
}

#include "ShaderProgram.gen.h"

#endif //GAME_SHADER_H
