//
// Created by bentoo on 7/11/16.
//

#ifndef GAME_MESHMANAGER_H
#define GAME_MESHMANAGER_H

// #include <Gfx/Api/BufferApi.h>
#include <Container.h>
#include "Mesh.h"

namespace Core
{
    class Engine;
}

namespace Resources
{
    class MeshManager
    {
        Core::Engine& engine;
        Utils::Container<Core::MeshTrait> meshes;

    public:
        MeshManager(Core::Engine& engine);

        auto createMesh() -> Core::MeshResource::handle_t;
        auto getMesh(Core::MeshResource::handle_t handle) -> Core::MeshResource&;
    //    auto uploadMesh(Core::MeshResource::handle_t handle, Gfx::BufferUsage usage) -> void;
    };
}

#endif //GAME_MESHMANAGER_H
