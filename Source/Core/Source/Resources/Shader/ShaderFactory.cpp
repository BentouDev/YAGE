//
// Created by mrjaqbq on 27.05.16.
//

#include <string>
#include <fstream>

// #include <Gfx/Api/ShaderApi.h>
#include <Context.h>

#include "Shader.h"

namespace Resources
{
	namespace ShaderFactory
	{
	/*	auto LoadShaderFromSource(std::string path, Gfx::Shader& shader, Core::Context& ctx, Gfx::ShaderApi& _api) -> void
		{
			std::ifstream f(path);
			std::string source;

			if(f.good())
			{
				f.seekg(0, std::ios::end);
				source.reserve(f.tellg());
				f.seekg(0, std::ios::beg);

				source.assign((std::istreambuf_iterator<char>(f)),
							   std::istreambuf_iterator<char>());

				_api.generateShader(shader, Gfx::ShaderApi::ShaderType::Vertex);
				_api.loadShaderSrc (shader, source);
				_api.compileShader (shader);
				_api.checkForErrors(shader, ctx);
			}
		}*/
	}
}