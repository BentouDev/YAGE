//
// Created by bentoo on 10/20/16.
//

#ifndef GAME_SHADERBUILDER_H
#define GAME_SHADERBUILDER_H

#include <Utils/MemoryBlock.h>
#include <Gfx/Graphics/OpenGl/OpenGLBase.h>

#include <string>

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
    public:
        using handle_t = Utils::Handle<Gfx::ShaderProgram>;

    protected:
        struct AttribInfo
        {
            AttribInfo(std::uint32_t loc, const char* attrib)
                : location(loc), name(attrib)
            { }

            std::uint32_t	location;
            const char*		name;
        };

        Core::Engine&				_engine;
        Memory::IMemoryBlock& 		_memory;
        Utils::List<Gfx::Shader*>	_shaders;
        Utils::List<Gfx::Shader*>	_temporaryShaders;
        Utils::List<AttribInfo>		_forcedAttributeLocation;
        Gfx::ShaderProgram*			_existing;

        void compileShader(Gfx::Shader& shader, const char* source, const GLint length);
        void checkForCompilationErrors(Gfx::Shader& shader);
        void checkForLinkErrors(Gfx::ShaderProgram& program);

    public:
        ShaderBuilder(Core::Engine& engine, Memory::IMemoryBlock& memory);
        ~ShaderBuilder();

        ShaderBuilder&	onExisting(Gfx::ShaderProgram& existing);
        ShaderBuilder&	onExisting(handle_t existing);

        ShaderBuilder&	with(Gfx::Shader& shader);

        ShaderBuilder&	withAttributeLocation(std::uint32_t location, const char* attribute);

        ShaderBuilder&	withVertexFromFile(const char* path);
        ShaderBuilder&	withVertexFromSource(const char* source);

        ShaderBuilder&	withFragmentFromFile(const char* path);
        ShaderBuilder&	withFragmentFromSource(const char* source);

        handle_t		debugBuild(const eastl::string& programName);
    };
}

#endif //GAME_SHADERBUILDER_H
