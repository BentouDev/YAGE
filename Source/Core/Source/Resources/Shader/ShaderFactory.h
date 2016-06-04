//
// Created by mrjaqbq on 27.05.16.
//

#ifndef GAME_SHADERFACTORY_H
#define GAME_SHADERFACTORY_H

#include <string>
#include "Shader.h"

namespace Resources
{
	namespace ShaderFactory
	{
		auto LoadShaderFromFile(std::string path, Gfx::Shader& shader) -> void;
		auto LinkShaderProgram(Gfx::ShaderProgram& prog, Gfx::Shader& shader) -> void;
	}
}


#endif //GAME_SHADERFACTORY_H
