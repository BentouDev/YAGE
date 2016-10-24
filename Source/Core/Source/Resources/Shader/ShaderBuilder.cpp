//
// Created by bentoo on 10/20/16.
//

#include <fstream>
#include <Engine.h>
#include "../../Gfx/OpenGl/OpenGLBase.h"
#include "../../Logger.h"
#include "ShaderBuilder.h"
#include "Shader.h"

namespace Resources
{
	ShaderBuilder::ShaderBuilder(Core::Engine& engine, Memory::IMemoryBlock &memory)
		: _engine(engine), _memory(memory), _temporaryShaders(_memory), _shaders(_memory)
	{

	}

	Gfx::ShaderProgram* ShaderBuilder::debugBuild(const char *programName)
	{
		Gfx::ShaderProgram* program = YAGE_CREATE_NEW(_memory, Gfx::ShaderProgram)();

		for(Gfx::Shader* shader : _shaders)
		{
			if(!shader->isCompiled())
			{
				_engine.Logger->Default->error("Unable to create shader program '{}', cause 'sub shader not compiled'", programName);
				return nullptr;
			}
		}

		for(Gfx::Shader& shader : _temporaryShaders)
		{
			if(!shader.isCompiled())
			{
				_engine.Logger->Default->error("Unable to create shader program '{}', cause 'sub shader not compiled'", programName);
				return nullptr;
			}
		}

		program->init(gl::CreateProgram());

		for(Gfx::Shader* shader : _shaders)
		{
			gl::AttachShader(*program, *shader);
		}

		for(Gfx::Shader& shader : _temporaryShaders)
		{
			gl::AttachShader(*program, shader);
		}

		gl::LinkProgram(*program);

		// TODO: Log error
		// _engine.Logger->Default->error("Unable to compile shader 'Fragment', cause '{}'");

		return program;
	}

	ShaderBuilder& ShaderBuilder::with(Gfx::Shader& shader)
	{
		_shaders.emplace(&shader);
		return *this;
	}

	ShaderBuilder& ShaderBuilder::withVertexFromSource(const char *source)
	{
		// new Shader
		Gfx::Shader& shader = _temporaryShaders.emplace(Gfx::ShaderType::VERTEX);

		// compile shader
		compileShader(shader, source, strlen(source));

		return *this;
	}

	ShaderBuilder& ShaderBuilder::withVertexFromFile(const char *path)
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

			return withVertexFromSource(source.c_str());
		}

		// TODO: log error

		return *this;
	}

	ShaderBuilder& ShaderBuilder::withFragmentFromSource(const char *source)
	{
		// new Shader
		Gfx::Shader& shader = _temporaryShaders.emplace(Gfx::ShaderType::FRAGMENT);

		// compile shader
		compileShader(shader, source, strlen(source));

		return *this;
	}

	ShaderBuilder& ShaderBuilder::withFragmentFromFile(const char *path)
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

			return withFragmentFromSource(source.c_str());
		}

		// TODO: log error

		return *this;
	}

	void ShaderBuilder::compileShader(Gfx::Shader &shader, const char *source, const GLint length)
	{
		gl::ShaderSource(shader, 1, &source, &length);
		gl::CompileShader(shader);

		checkForCompilationErrors(shader);
	}

	void ShaderBuilder::checkForCompilationErrors(Gfx::Shader& shader)
	{
		int compileResult;

		gl::GetShaderiv(shader, gl::COMPILE_STATUS, &compileResult);

		if (compileResult == 0)
		{
			int bufferSize;
			gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &bufferSize);

			Utils::String string(_memory);
			string.reserve(bufferSize);

			gl::GetShaderInfoLog(shader, bufferSize, &bufferSize, string.begin());

			// TODO what to do with such cases? not builded shaders should be properly disposed
			// if its our shader it will be deleted anyway
			// if its external, we dont care

			_engine.Logger->Default->error("Unable to compile shader '{}', cause '{}'", shader._type, string.c_str());

			shader._isCompiled = false;
		}
		else
		{
			shader._isCompiled = true;
		}
	}
}
