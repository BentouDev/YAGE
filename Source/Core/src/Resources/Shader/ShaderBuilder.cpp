//
// Created by bentoo on 10/20/16.
//

#include <fstream>
#include <Utils/String.h>

#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Core/Resources/Shader/ShaderBuilder.h"
#include "Core/Resources/Shader/ShaderManager.h"

namespace Resources
{
	ShaderBuilder::ShaderBuilder(Core::Engine& engine, Memory::IMemoryBlock &memory)
		: _engine(engine), _memory(memory), _shaders(_memory), _temporaryShaders(_memory), _existing(nullptr)
	{

	}

	ShaderBuilder::handle_t ShaderBuilder::debugBuild(const char *programName)
	{
		Gfx::ShaderProgram* program = nullptr;

		if(_existing != nullptr)
		{
			program = _existing;
			if((*program) > 0)
			{
				gl::DeleteProgram((*program));
			}
		}
		else
		{
			handle_t handle = _engine.ShaderManager.get().createNew();
			program = &_engine.ShaderManager.get().get(handle);
		}

		program->Name = programName;

		for(Gfx::Shader* shader : _shaders)
		{
			if(!shader->isCompiled())
			{
				Core::Logger::get()->error("Unable to create shader program '{}', cause 'sub shader not compiled'", programName);
				return handle_t::invalid();
			}
		}

		for(Gfx::Shader& shader : _temporaryShaders)
		{
			if(!shader.isCompiled())
			{
				Core::Logger::get()->error("Unable to create shader program '{}', cause 'sub shader not compiled'", programName);
				return handle_t::invalid();
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
		checkForLinkErrors(*program);

		return program->Handle;
	}

	ShaderBuilder& ShaderBuilder::onExisting(handle_t existing)
	{
		_existing = _engine.ShaderManager.get().tryGet(existing);
		return *this;
	}

	ShaderBuilder& ShaderBuilder::onExisting(Gfx::ShaderProgram& existing)
	{
		_existing = &existing;
		return *this;
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

		if (compileResult == gl::FALSE_)
		{
			int bufferSize;
			gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &bufferSize);

			Utils::String string(_memory);
			string.reserve(bufferSize);

			gl::GetShaderInfoLog(shader, bufferSize, &bufferSize, string.begin());

			Core::Logger::get()->error("Unable to compile shader '{}', cause :\n{}", shader._type, string.c_str());

			shader._isCompiled = false;
		}
		else
		{
			shader._isCompiled = true;
		}
	}

	void ShaderBuilder::checkForLinkErrors(Gfx::ShaderProgram &program)
	{
		int linkStatus;

		gl::GetProgramiv(program, gl::LINK_STATUS, &linkStatus);

		if(linkStatus == gl::FALSE_)
		{
			int bufferSize;
			gl::GetProgramiv(program, gl::INFO_LOG_LENGTH, &bufferSize);

			Utils::String string(_memory);
			string.reserve(bufferSize);

			gl::GetProgramInfoLog(program, bufferSize, &bufferSize, string.begin());

			Core::Logger::get()->error("Unable to link shader program '{}', cause :\n{}", program.Name, string.c_str());
		}
	}
}
