//
// Created by bentoo on 09.11.16.
//

#ifndef GAME_SHADERMANAGER_H
#define GAME_SHADERMANAGER_H

#include <Utils/MemoryBlock.h>
#include <Utils/Handle.h>
#include <Utils/Colony.h>

#include "Shader.h"
#include "Core/IManager.h"

namespace Core
{
    class Engine;
}

namespace Resources
{
    class ShaderManager : public Core::IManager
    {
        MANAGER(ShaderManager);

    public:
        using handle_t = Utils::Handle<Gfx::ShaderProgram>;

    protected:
        Utils::Colony<Gfx::ShaderProgram> _shadersContainer;

        void dispose(Gfx::ShaderProgram&);

    public:
        explicit ShaderManager(Core::Engine& engine, Memory::IMemoryBlock& memory);
        virtual ~ShaderManager();

        void disposeAll();
        void dispose(handle_t);

        handle_t createNew()
        { return _shadersContainer.emplace(); }

        Gfx::ShaderProgram& get(handle_t handle)
        { return _shadersContainer.get(handle); }

        const Gfx::ShaderProgram& get(handle_t handle) const
        { return _shadersContainer.get(handle); }

        Gfx::ShaderProgram* tryGet(handle_t);

        const Gfx::ShaderProgram* tryGet(handle_t) const;

        bool contains(handle_t handle) const
        { return _shadersContainer.contains(handle); }
    };
}

#endif //GAME_SHADERMANAGER_H
