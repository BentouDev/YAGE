//
// Created by bentoo on 10/17/16.
//

#ifndef GAME_SHADERBUILDER_H
#define GAME_SHADERBUILDER_H

#include <Utils/List.h>
#include <Resources/Shader/Shader.h>

namespace Gfx
{
	class ShaderBuilder
	{
		Utils::List<Shader> _shaders;

	public:
		ShaderBuilder& addShader(const Shader& shader);
		ShaderBuilder& addShader(Shader* shader);
		ShaderBuilder& addShader(Gfx::ShaderType type);
	};
}

#endif //GAME_SHADERBUILDER_H
