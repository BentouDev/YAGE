//
// Created by mrjaqbq on 27.05.16.
//

#ifndef GAME_SHADERAPI_H
#define GAME_SHADERAPI_H

#include <string>

namespace Core
{
	class Context;
}

namespace Gfx
{
	class Shader;

	class ShaderApi
	{
	public:

		enum ShaderType
		{
			Vertex = 0,
			TesselationControl = 1,
			TesselationEvaluation = 2,
			Geometry = 3,
			Fragment = 4,
			Compute = 5
		};

		virtual auto generateShader(Gfx::Shader& shader, Gfx::ShaderApi::ShaderType type) -> void = 0;
		virtual auto loadShaderSrc(Gfx::Shader& shader, std::string src) -> void = 0;
		virtual auto compileShader(Gfx::Shader& shader) -> void = 0;
		virtual auto checkForErrors(Gfx::Shader& shader, Core::Context& ctx) -> bool = 0;
	};
}

#endif //GAME_SHADERAPI_H
