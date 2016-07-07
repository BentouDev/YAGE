//
// Created by mrjaqbq on 27.05.16.
//

#include "Generated/gl_core_3_3.hpp"
#include "OpenGLShader.h"

#include <Platform.h>
#include <Resources/Shader/Shader.h>
#include <Context.h>
#include <Logger.h>

namespace Gfx
{
	auto OpenGLShader::generateShader(Gfx::Shader &shader, ShaderType type) -> void
	{
		shader.apiHandle = gl::CreateShader(shaderTypes[type]);
	}

	auto OpenGLShader::compileShader(Gfx::Shader &shader) -> void
	{
		gl::CompileShader(shader.apiHandle);
	}

	auto OpenGLShader::loadShaderSrc(Gfx::Shader &shader, std::string src) -> void
	{
		int len = (int) src.length();
		auto ptr = src.c_str();
		gl::ShaderSource(shader.apiHandle, 1, &ptr, &len);
	}

	auto OpenGLShader::checkForErrors(Gfx::Shader& shader, Core::Context& ctx) -> bool
	{
		int compileResult;
		gl::GetShaderiv(shader.apiHandle, gl::COMPILE_STATUS, &compileResult);

		if (compileResult == gl::FALSE_)
		{
			GLint maxLength = 0;
			gl::GetShaderiv(shader.apiHandle, gl::INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog((unsigned long) maxLength);
			gl::GetShaderInfoLog(shader.apiHandle, maxLength, &maxLength, &errorLog[0]);

			gl::DeleteShader(shader.apiHandle);

			ctx.Logger->Default->warn(reinterpret_cast<const char*>(&errorLog[0]));

			return false;
		}

		return true;
	}
}