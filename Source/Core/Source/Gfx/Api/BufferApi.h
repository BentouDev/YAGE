//
// Created by bentoo on 7/7/16.
//

#ifndef GAME_BUFFERAPI_H_H
#define GAME_BUFFERAPI_H_H

#include <cstdint>

namespace Gfx
{
    class BufferData;

    enum BufferUsage
    {
        STATIC_READ,
        STATIC_DRAW,
        STATIC_COPY,
        DYNAMIC_READ,
        DYNAMIC_DRAW,
        DYNAMIC_COPY,
        STREAM_READ,
        STREAM_DRAW,
        STREAM_COPY
    };

    class BufferApi
    {
    public:
        auto virtual CreateBuffer() -> uint32_t = 0;
        auto virtual UploadBuffer(uint32_t apiKey, Gfx::BufferData& data, BufferUsage usage) -> void = 0;
        auto virtual DeleteBuffer(uint32_t apiKey) -> void = 0;
    };
}

#endif //GAME_BUFFERAPI_H_H
