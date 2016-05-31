//
// Created by mrjaqbq on 21.05.16.
//

#ifndef GAME_SHADER_H
#define GAME_SHADER_H

#include <map>
#include <vector>

#include <DefaultTrait.h>

#include "../Resource.h"
#include "../../Gfx/Api/ShaderApi.h"

namespace Gfx
{
	class Shader;
	class ShaderProgram;
	class ShaderResource;

	class ShaderTrait : public Utils::DefaultTrait<Shader> {};
	class ShaderProgTrait : public Utils::DefaultTrait<ShaderProgram> {};
	class ShaderResTrait : public Utils::DefaultTrait<ShaderResource> {};

	class Shader
	{
	public:
		using api_handle = unsigned long long;

		api_handle apiHandle;
		ShaderApi::ShaderType type;
	};

	class ShaderProgram
	{
		// various shaders linked togehter

	public:
		using api_handle = unsigned long long;

		api_handle apiHandle;
	};

	DECL_RESOURCE(ShaderResource)
	{
	public:
		ShaderProgTrait::handle program;
		std::map<ShaderApi::ShaderType, std::vector<ShaderTrait::handle>> shaderMap;
	};
}

#endif //GAME_SHADER_H
