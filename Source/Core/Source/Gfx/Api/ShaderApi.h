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

		virtual auto generateShader(Gfx::Shader& shader) -> void;
		virtual auto loadShaderSrc(Gfx::Shader& shader, std::string src) -> void;
		virtual auto compileShader(Gfx::Shader& shader) -> void;
		virtual auto checkForErrors(Gfx::Shader& shader, Core::Context& ctx) -> bool;
	};
}

#endif //GAME_SHADERAPI_H
