//
// Created by bentoo on 7/8/16.
//

#include <DefaultTrait.h>

#ifndef GAME_OPENGLMESHTRAIT_H
#define GAME_OPENGLMESHTRAIT_H

namespace Gfx
{
    struct OpenGLMeshData;

    class OpenGLMeshTrait : public Utils::DefaultTrait<OpenGLMeshData>
    { };

    struct OpenGLMeshData
    {
    public:
        using handle_t = OpenGLMeshTrait::handle;

        handle_t Handle;

        void swap(OpenGLMeshData& other) noexcept { }
        void cleanUp() noexcept { }
    };
}

#endif //GAME_OPENGLMESHTRAIT_H
