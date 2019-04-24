//
// Created by bentoo on 10/20/16.
//

#include <fstream>

#include <Utils/String.h>
#include <Platform/Logger.h>

#include "Core/Engine.h"
#include "Core/Resources/Shader/ShaderBuilder.h"
#include "Core/Resources/Shader/ShaderManager.h"

namespace Resources
{
    ShaderBuilder::ShaderBuilder(Core::Engine& engine, Memory::IMemoryBlock &memory)
        : _engine(engine),
          _memory(memory),
          _shaders(),
          _temporaryShaders(),
          _forcedAttributeLocation(), // #NewAlloc
          _existing(nullptr)
    {

    }

    ShaderBuilder::~ShaderBuilder()
    {
        for (auto* shader : _temporaryShaders)
        {
            Memory::Delete(_memory, shader);
        }
    }

    ShaderBuilder::handle_t ShaderBuilder::debugBuild(const eastl::string& programName)
    {
        Gfx::ShaderProgram* program = nullptr;

        if (_existing != nullptr)
        {
            program = _existing;
            if ((*program) > 0)
            {
                gl::DeleteProgram((*program));
            }
        }
        else
        {
            handle_t handle = _engine.ShaderManager->createNew();
            program = &_engine.ShaderManager->get(handle);
        }

        program->Name = programName;

        for (Gfx::Shader* shader : _shaders)
        {
            if (!shader->isCompiled())
            {
                Core::Logger::error("Unable to create shader program '{}', cause 'sub shader not compiled'", programName.c_str());
                return handle_t::invalid();
            }
        }

        for (Gfx::Shader* shader : _temporaryShaders)
        {
            if (!shader->isCompiled())
            {
                Core::Logger::error("Unable to create shader program '{}', cause 'sub shader not compiled'", programName.c_str());
                return handle_t::invalid();
            }
        }

        program->init(gl::CreateProgram());
        
        // enforce attrib location
        for (auto& attrib : _forcedAttributeLocation)
        {
            gl::BindAttribLocation(*program, attrib.location, attrib.name);
        }

        for (Gfx::Shader* shader : _shaders)
        {
            gl::AttachShader(*program, *shader);
        }

        for (Gfx::Shader* shader : _temporaryShaders)
        {
            gl::AttachShader(*program, *shader);
        }

        gl::LinkProgram(*program);
        checkForLinkErrors(*program);

        return Utils::handle_cast<Gfx::ShaderProgram>(program->Handle);
    }

    ShaderBuilder& ShaderBuilder::onExisting(handle_t existing)
    {
        _existing = _engine.ShaderManager->tryGet(existing);
        return *this;
    }

    ShaderBuilder& ShaderBuilder::onExisting(Gfx::ShaderProgram& existing)
    {
        _existing = &existing;
        return *this;
    }

    ShaderBuilder& ShaderBuilder::with(Gfx::Shader& shader)
    {
        _shaders.emplace_back(&shader);
        return *this;
    }

    ShaderBuilder& ShaderBuilder::withAttributeLocation(std::uint32_t location, const char* attribute)
    {
        _forcedAttributeLocation.emplace_back(location, attribute);
        return *this;
    }

    ShaderBuilder& ShaderBuilder::withVertexFromSource(const char *source)
    {
        // new Shader
        Gfx::Shader* shader = _temporaryShaders.emplace_back(YAGE_CREATE_NEW(_memory, Gfx::Shader)(Gfx::ShaderType::VERTEX));

        // compile shader
        compileShader(*shader, source, strlen(source));

        return *this;
    }

    ShaderBuilder& ShaderBuilder::withVertexFromFile(const char *path)
    {
        std::ifstream f(path);
        std::string source;

        if (f.good())
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
        Gfx::Shader* shader = _temporaryShaders.emplace_back(YAGE_CREATE_NEW(_memory, Gfx::Shader)(Gfx::ShaderType::FRAGMENT));

        // compile shader
        compileShader(*shader, source, strlen(source));

        return *this;
    }

    ShaderBuilder& ShaderBuilder::withFragmentFromFile(const char *path)
    {
        std::ifstream f(path);
        std::string source;

        if (f.good())
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

			Utils::String string{};// #NewAlloc
            string.reserve(bufferSize);

            gl::GetShaderInfoLog(shader, bufferSize, &bufferSize, string.begin());

            Core::Logger::error("Unable to compile shader '{}', cause :\n{}", shader._type, string.c_str());

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

        if (linkStatus == gl::FALSE_)
        {
            int bufferSize;
            gl::GetProgramiv(program, gl::INFO_LOG_LENGTH, &bufferSize);

			Utils::String string{}; // #NewAlloc
            string.reserve(bufferSize);

            gl::GetProgramInfoLog(program, bufferSize, &bufferSize, string.begin());

            Core::Logger::error("Unable to link shader program '{}', cause :\n{}", program.Name.c_str(), string.c_str());
        }
    }
}
