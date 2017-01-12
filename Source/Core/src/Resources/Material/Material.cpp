//
// Created by mrjaqbq on 13.05.16.
//

#include <glm/gtc/type_ptr.hpp>

#include "Core/Resources/Texture/Texture.h"
#include "Core/Resources/Material/Material.h"
#include "Core/Resources/Shader/Shader.h"
#include "Core/Resources/Shader/ShaderManager.h"

namespace Core
{
	void Material::setUniform(GLint location, float value)
	{
		gl::Uniform1f(location, value);
	}

	void Material::setUniform(GLint location, int value)
	{
		gl::Uniform1i(location, value);
	}

	void Material::setUniform(GLint location, glm::vec2 value)
	{
		gl::Uniform2f(location, value.x, value.y);
	}

	void Material::setUniform(GLint location, glm::vec3 value)
	{
		gl::Uniform3f(location, value.x, value.y, value.z);
	}

	void Material::setUniform(GLint location, glm::vec4 value)
	{
		gl::Uniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Material::setUniform(GLint location, glm::mat4x4 value)
	{
		gl::UniformMatrix4fv(location, 1, gl::FALSE_, glm::value_ptr(value));
	}

	void Material::setUniform(GLint location, Utils::Color value)
	{
		gl::Uniform4f(location, value.R / 255.f, value.G / 255.f, value.B / 255.f, value.A / 255.f);
	}

	void Material::setUniform(GLint location, Resources::Texture* texture)
	{
		gl::ActiveTexture(gl::TEXTURE0 + _textureIndex);
		gl::BindTexture(gl::TEXTURE_2D, *texture);
		gl::Uniform1ui(location, _textureIndex);
		_textureIndex++;
	}

	IAutoUniform::IAutoUniform(const char* name,
							   Core::Material& material,
							   const Resources::ShaderManager& manager)
		: _material(material)
	{
		_location = _material.getLocation(name, manager);
	}

	GLint Material::getLocation(const char* name, const Resources::ShaderManager& manager) const
	{
		return gl::GetUniformLocation(manager.get(getShaderProgram()), name);
	}
}
