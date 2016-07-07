//
// Created by bentoo on 7/7/16.
//

#include <cstdint>
#include <Gfx/Buffer.h>
#include <Gfx/OpenGl/Generated/gl_core_3_3.hpp>
#include "OpenGLBuffer.h"

namespace Gfx
{
    uint32_t OpenGLBuffer::CreateBuffer()
    {
        uint32_t result;
        gl::GenBuffers(1, &result);

        return result;
    }

    void OpenGLBuffer::UploadBuffer(uint32_t apiKey, Gfx::BufferData& data, BufferUsage usage)
    {
        gl::BindBuffer(gl::ARRAY_BUFFER, apiKey);
        gl::BufferData(gl::ARRAY_BUFFER, data.size(), data.getDataPtr(), OpenGLBufferUsage[usage]);
    }

    void OpenGLBuffer::DeleteBuffer(uint32_t apiKey)
    {
        gl::DeleteBuffers(1, &apiKey);
    }
}