//
// Created by bentoo on 09.11.16.
//

#include "Core/Resources/Shader/ShaderManager.h"
#include "Core/Engine.h"
#include "Core/Logger.h"

namespace Resources
{
    ShaderManager::ShaderManager(Core::Engine &engine, Memory::IMemoryBlock &memory)
     : IManager(engine, memory), _shadersContainer(_memory)
    {
        Core::Logger::info("Created shader manager with capacity {}", _shadersContainer.capacity());
    }

    ShaderManager::~ShaderManager()
    {
        disposeAll();
    }

    void ShaderManager::disposeAll()
    {
        Core::Logger::info("Releasing all shaders...");

        _shadersContainer.clear();

        Core::Logger::info("Released all shaders");
    }

    void ShaderManager::dispose(handle_t handle)
    {
        _shadersContainer.erase(handle);
    }

    void ShaderManager::dispose(Gfx::ShaderProgram& program)
    {
        dispose(program.Handle);
    }

    Gfx::ShaderProgram* ShaderManager::tryGet(handle_t handle)
    {
        Gfx::ShaderProgram* ptr = nullptr;
        if(_shadersContainer.contains(handle))
        {
            ptr = &get(handle);
        }
        return ptr;
    }

    const Gfx::ShaderProgram* ShaderManager::tryGet(handle_t handle) const
    {
        const Gfx::ShaderProgram* ptr = nullptr;
        if(_shadersContainer.contains(handle))
        {
            ptr = &get(handle);
        }
        return ptr;
    }
}
