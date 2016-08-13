//
// Created by bentoo on 7/7/16.
//

#ifndef GAME_MESHAPI_H
#define GAME_MESHAPI_H

#include <Handle.h>

namespace Gfx
{
    // we will have one type of handle to index container
    // which will contain api specific data
    // like opengl structs with handle, index and vao!

    struct ApiMeshData
    { };

    class MeshApi
    {
    public:
        using ApiMeshHandle = Utils::Handle<ApiMeshData>;

        virtual auto createApiMeshData() -> ApiMeshHandle = 0;
        virtual auto removeApiMeshData(ApiMeshHandle handle) -> void = 0;
        virtual auto containsApiMeshData(ApiMeshHandle handle) -> bool = 0;
    };
}

#endif //GAME_MESHAPI_H
