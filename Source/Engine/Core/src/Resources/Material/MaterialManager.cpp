//
// Created by bentoo on 10/28/16.
//

#include "Core/Resources/Material/MaterialManager.h"
#include "Core/Engine.h"
#include "Core/Logger.h"

namespace Resources
{
    MaterialManager::MaterialManager(Core::Engine& engine, Memory::IMemoryBlock& memory)
        : IManager(engine, memory), _materialContainer(_memory)
    {
        Core::Logger::info("Created material manager with capacity {}", _materialContainer.capacity());
    }

    MaterialManager::~MaterialManager() noexcept
    {
        disposeAll();
    }

    MaterialManager::smart_t MaterialManager::createMaterial()
    {
        return Utils::make_handle(&_materialContainer, _memory);
    }

    void MaterialManager::disposeAll()
    {
        Core::Logger::info("Releasing all materials...");

        _materialContainer.clear();

        Core::Logger::info("Released all materials");
    }

    Core::Material* MaterialManager::tryGetMaterial(handle_t handle)
    {
        Core::Material* ptr = nullptr;
        if(_materialContainer.contains(handle))
        {
            ptr = &getMaterial(handle);
        }
        return ptr;
    }

    void MaterialManager::disposeMaterial(Core::Material &material)
    {
        disposeMaterial(Utils::handle_cast<Core::Material>(material.Handle));
    }

    void MaterialManager::disposeMaterial(handle_t handle)
    {
        // TODO: ADDITIONAL THINGS, LIKE STATIC BUFFER INVALIDATION

        _materialContainer.erase(handle);
    }

    void MaterialManager::uploadToBuffer(handle_t handle)
    {
        Core::Material* material = tryGetMaterial(handle);

        if(material == nullptr)
            return;


    }
}
