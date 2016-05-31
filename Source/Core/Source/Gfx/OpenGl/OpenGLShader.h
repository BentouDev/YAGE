//
// Created by mrjaqbq on 27.05.16.
//

#ifndef GAME_OPENGLSHADER_H
#define GAME_OPENGLSHADER_H

#include "Generated/gl_core_3_3.hpp"
#include <Gfx/Api/ShaderApi.h>

namespace Core
{
	class Context;
}

namespace Gfx
{
	class OpenGLShader : public ShaderApi
	{
		static constexpr GLenum shaderTypes[6] = {
			gl::VERTEX_SHADER,
			0,
			0,
			gl::GEOMETRY_SHADER,
			gl::FRAGMENT_SHADER,
			0
		};

	public:
		virtual auto generateShader(Gfx::Shader& shader, Gfx::ShaderApi::ShaderType type) -> void;

		virtual auto loadShaderSrc(Gfx::Shader& shader, std::string src) -> void;

		virtual auto compileShader(Gfx::Shader& shader) -> void;

		virtual auto checkForErrors(Gfx::Shader& shader, Core::Context& ctx) -> bool;
	};
}

#endif //GAME_OPENGLSHADER_H
