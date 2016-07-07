//
// Created by bentoo on 7/7/16.
//

#ifndef GAME_OPENGGLBUFFER_H
#define GAME_OPENGGLBUFFER_H

#include "Gfx/Api/BufferApi.h"

namespace Gfx
{
    GLenum OpenGLBufferUsage[] = {
        gl::STATIC_READ,
        gl::STATIC_DRAW,
        gl::STATIC_COPY,
        gl::DYNAMIC_READ,
        gl::DYNAMIC_DRAW,
        gl::DYNAMIC_COPY,
        gl::STREAM_READ,
        gl::STREAM_DRAW,
        gl::STREAM_COPY,
    };

    class OpenGLBuffer : BufferApi
    {
    public:
        uint32_t CreateBuffer() override;
        void UploadBuffer(uint32_t apiKey, Gfx::BufferData& data, BufferUsage usage) override;
        void DeleteBuffer(uint32_t apiKey) override;
    };
}

#endif //GAME_OPENGGLBUFFER_H
