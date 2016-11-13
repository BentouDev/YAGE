//
// Created by mrjaqbq on 13.05.16.
//

#include <glm/gtc/type_ptr.hpp>

#include "Core/Resources/Material/Material.h"
#include "Core/Resources/Shader/Shader.h"
#include "Core/Resources/Shader/ShaderManager.h"

namespace Gfx
{
	void setUniform(GLint location, float value)
	{
		gl::Uniform1f(location, value);
	}

	void setUniform(GLint location, int value)
	{
		gl::Uniform1i(location, value);
	}

	void setUniform(GLint location, glm::vec2 value)
	{
		gl::Uniform2f(location, value.x, value.y);
	}

	void setUniform(GLint location, glm::vec3 value)
	{
		gl::Uniform3f(location, value.x, value.y, value.z);
	}

	void setUniform(GLint location, glm::vec4 value)
	{
		gl::Uniform4f(location, value.x, value.y, value.z, value.w);
	}

	void setUniform(GLint location, glm::mat4x4 value)
	{
		gl::UniformMatrix4fv(location, 1, gl::FALSE_, glm::value_ptr(value));
	}

	void setUniform(GLint location, Utils::Color value)
	{
		gl::Uniform4f(location, value.R / 255.f, value.G / 255.f, value.B / 255.f, value.A / 255.f);
	}
}

namespace Core
{
	IAutoUniform::IAutoUniform(const char* name, const Core::Material& material, const Resources::ShaderManager& manager)
	{
		Gfx::ShaderProgram& program = manager.get(material.getShaderProgram());
		_location = gl::GetUniformLocation(program, name);
	}
}
