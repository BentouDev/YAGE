//
// Created by bentoo on 10/20/16.
//

#ifndef GAME_SHADERBUILDER_H
#define GAME_SHADERBUILDER_H

#include <Utils/String.h>

namespace Gfx
{
	class Shader;
	class ShaderProgram;
}

namespace Core
{
	class Engine;
}

namespace Resources
{
	class ShaderBuilder
	{
		Core::Engine&				_engine;
		Memory::IMemoryBlock& 		_memory;
		Utils::List<Gfx::Shader*>	_shaders;
		Utils::List<Gfx::Shader>	_temporaryShaders;

		void compileShader(Gfx::Shader& shader, const char* source, const GLint length);
		void checkForCompilationErrors(Gfx::Shader& shader);
		void checkForLinkErrors(Gfx::ShaderProgram& program);

	public:
		ShaderBuilder(Core::Engine& engine, Memory::IMemoryBlock& memory);

		ShaderBuilder&	with(Gfx::Shader& shader);

		ShaderBuilder&	withVertexFromFile(const char* path);
		ShaderBuilder&	withVertexFromSource(const char* source);

		ShaderBuilder&	withFragmentFromFile(const char* path);
		ShaderBuilder&	withFragmentFromSource(const char* source);

		Gfx::ShaderProgram*	debugBuild(const char* programName);
	};
}

#endif //GAME_SHADERBUILDER_H
