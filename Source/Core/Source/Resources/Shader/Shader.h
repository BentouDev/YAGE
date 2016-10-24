//
// Created by mrjaqbq on 21.05.16.
//

#ifndef GAME_SHADER_H
#define GAME_SHADER_H

#include <Utils/DefaultTrait.h>
#include <Utils/List.h>
#include "../../Gfx/OpenGl/OpenGLBase.h"

#include "../Resource.h"

namespace Resources
{
	class ShaderBuilder;
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
		ShaderType	_type;
		GLuint 		_handle;
		bool 		_isCompiled;

		friend class Resources::ShaderBuilder;

	public:
		explicit inline Shader(ShaderType type) : _type(type), _isCompiled(false)
		{
			_handle = gl::CreateShader(type);
		}

		~Shader() noexcept
		{
			gl::DeleteShader(_handle);
		}

		Shader(const Shader& shader)
			: _type(shader._type), _handle(shader._handle)
		{ }

		Shader(Shader&& shader)
			: _type(shader._type), _handle(shader._handle)
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
			other._handle = -1;
			return *this;
		}

		inline operator GLint() const noexcept
		{ return _handle; }

		inline bool isCompiled() const noexcept
		{ return _isCompiled; }
	};

	DECL_RESOURCE(ShaderProgram)
	{
		GLuint _handle;

	public:
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram() : _handle(-1)
		{ }

		void init(GLint handle);

		void cleanUp() noexcept;

		inline void swap(ShaderProgram& other) noexcept
		{
			_handle = other._handle;
			assert(false && "SWAP IN SHADER NOT IMPLEMENTED");
		}

		inline operator GLuint() const noexcept
		{ return _handle; }

		inline void bind() const noexcept;
	};

	class ShaderProgTrait : public Utils::DefaultTrait<ShaderProgram> {};
}

#endif //GAME_SHADER_H
