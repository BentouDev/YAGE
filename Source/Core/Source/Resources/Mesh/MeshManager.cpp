//
// Created by bentoo on 7/11/16.
//

#include "MeshManager.h"
#include <Engine.h>
// #include <Gfx/Api/MeshApi.h>

namespace Resources
{
/*    MeshManager::MeshManager(Core::Engine &engine) : engine(engine)
    {

    }

    auto MeshManager::createMesh() -> Core::MeshResource::handle_t
    {
        return meshes.create();
    }

    auto MeshManager::getMesh(Core::MeshResource::handle_t handle) -> Core::MeshResource&
    {
        return meshes.get(handle);
    }*/

    /*auto MeshManager::uploadMesh(Core::MeshResource::handle_t handle, Gfx::BufferUsage usage) -> void
    {
        Core::MeshResource& mesh = meshes.get(handle);

        Gfx::MeshApi* api = engine.GetApi().getMeshApi();
        for(auto sub : mesh.Submeshes)
        {
            if(!api->containsApiMeshData(sub.getApiHandle()))
            {
                sub.setApiHandle(api->createApiMeshData());
                sub.uploadApiData(api);
            }
        }
    }*/
}