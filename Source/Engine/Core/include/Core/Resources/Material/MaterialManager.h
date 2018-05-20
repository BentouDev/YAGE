//
// Created by bentoo on 10/28/16.
//

#ifndef GAME_MATERIALMANAGER_H
#define GAME_MATERIALMANAGER_H

#include <Utils/MemoryBlock.h>
#include <Utils/Container.h>
#include <Utils/SmartHandle.h>
#include "Material.h"
#include "Core/IManager.h"

namespace Core
{
    class Engine;
}

namespace Resources
{
    class MaterialManager : public Core::IManager
    {
        MANAGER(MaterialManager);

        using handle_t = Core::Material::handle_t;
        using smart_t  = Utils::SmartHandle<Core::MaterialTrait>;

        Utils::Container<Core::MaterialTrait> _materialContainer;

        void disposeMaterial(Core::Material& material);

    public:
        explicit MaterialManager(Core::Engine& engine, Memory::IMemoryBlock& memory);
        virtual ~MaterialManager() noexcept;

        MaterialManager(const MaterialManager&) = delete;
        MaterialManager(MaterialManager&&) = delete;

        void disposeAll();
        void reloadAll();

        void disposeMaterial(handle_t material);

        Core::Material* tryGetMaterial(handle_t);

        smart_t createMaterial();

        void uploadToBuffer(handle_t handle);

        bool hasMesh(handle_t handle)
        { return _materialContainer.contains(handle); }

        Core::Material& getMaterial(handle_t handle)
        { return _materialContainer.get(handle); }

        const Core::Material& getMaterial(handle_t handle) const
        { return _materialContainer.get(handle); }
    };
}

#endif //GAME_MATERIALMANAGER_H
