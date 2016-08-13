//
// Created by bentoo on 7/8/16.
//

#ifndef GAME_OPENGLMESH_H
#define GAME_OPENGLMESH_H

#include <Container.h>
#include "Gfx/Api/MeshApi.h"
#include "OpenGLMeshTrait.h"

namespace Gfx
{
    class OpenGLMesh : public MeshApi
    {
        Utils::Container<OpenGLMeshTrait> dataContainer;

    public:
        MeshApi::ApiMeshHandle createApiMeshData();
        void removeApiMeshData(MeshApi::ApiMeshHandle handle);
        bool containsApiMeshData(MeshApi::ApiMeshHandle handle);
    };
}

#endif //GAME_OPENGLMESH_H
