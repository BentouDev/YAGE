//
// Created by Bentoo on 2017-04-13.
//

#include <Utils/MemoryBlock.h>
#include <Platform/Logger.h>

#include "Core/Gfx/SpriteBatchManager.h"
#include "Core/Gfx/SpriteBatch.h"
#include "Core/Gfx/Sprite.h"
#include "Core/Gfx/Camera.h"
#include "Core/Resources/Material/MaterialManager.h"
#include "Core/Engine.h"

namespace Gfx
{
    const std::int32_t SpriteBuffer::DEFAULT_BUFFER_SIZE = 1000;
    const std::int32_t SpriteBuffer::MAX_BUFFER_SIZE = 64*1000;

    SpriteBuffer::SpriteBuffer(Memory::IMemoryBlock& memory, std::int32_t size)
        : _mappedPtr(nullptr), _vbo(nullptr), maximumSize(size), currentSize(0), offset(0)
    {
        _vbo = OpenGL::VBO::Create(memory);

        gl::BindBuffer(gl::ARRAY_BUFFER, *_vbo);
        OpenGL::checkError();

        gl::BufferData(gl::ARRAY_BUFFER, maximumSize * sizeof(SpriteVertex), nullptr, gl::DYNAMIC_DRAW);
        OpenGL::checkError();

        _vao = OpenGL::VAO::Create(memory);

        gl::BindVertexArray(getVAO());

        Core::Logger::info("SpriteBatchManager : creating sprite VAO...");

        gl::EnableVertexAttribArray(0);
        OpenGL::checkError();
        gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, sizeof(SpriteVertex), reinterpret_cast<void*>(0));
        OpenGL::checkError();

        gl::EnableVertexAttribArray(1);
        OpenGL::checkError();
        auto textureOffset = sizeof(glm::vec3);
        gl::VertexAttribPointer(1, 3, gl::FLOAT, gl::FALSE_, sizeof(SpriteVertex), reinterpret_cast<void*>(textureOffset));
        OpenGL::checkError();

        gl::EnableVertexAttribArray(2);
        OpenGL::checkError();
        auto colorOffset = sizeof(glm::vec3) * 2;
        gl::VertexAttribPointer(2, 4, gl::UNSIGNED_BYTE, gl::TRUE_, sizeof(SpriteVertex), reinterpret_cast<void*>(colorOffset));
        OpenGL::checkError();

        Core::Logger::info("SpriteBatchManager : Created sprite VAO!");

        gl::BindVertexArray(0);
        gl::BindBuffer(gl::ARRAY_BUFFER, 0);
    }

    void SpriteBuffer::clear()
    {
        offset = 0;
        currentSize = 0;
        unmapMemory();
        mapMemory();
    }

    void SpriteBuffer::mapMemory()
    {
        if (_mappedPtr != nullptr)
            return;

        gl::BindBuffer(gl::ARRAY_BUFFER, getVBO());
        OpenGL::checkError();

        _mappedPtr = gl::MapBufferRange(gl::ARRAY_BUFFER, 0, maximumSize * sizeof(SpriteVertex),
                                        gl::MAP_WRITE_BIT | gl::MAP_INVALIDATE_RANGE_BIT);
        OpenGL::checkError();

        gl::BindBuffer(gl::ARRAY_BUFFER, 0);
        OpenGL::checkError();
    }

    void SpriteBuffer::unmapMemory()
    {
        if (_mappedPtr == nullptr)
            return;

        _mappedPtr = nullptr;
        gl::BindBuffer (gl::ARRAY_BUFFER, getVBO());
        OpenGL::checkError();

        gl::UnmapBuffer(gl::ARRAY_BUFFER);
        OpenGL::checkError();

        gl::BindBuffer (gl::ARRAY_BUFFER, 0);
        OpenGL::checkError();
    }

    void SpriteBuffer::copyData(SpriteVertex (&array)[6])
    {
        SpriteVertex* buffer = static_cast<SpriteVertex*>(_mappedPtr);
        memcpy(buffer, array, sizeof(SpriteVertex) * 6);

        buffer		+= 6;
        currentSize	+= 6;
        _mappedPtr	 = buffer;
    }

    SpriteBatchManager::SpriteBatchManager(Core::Engine& engine,
                                           Memory::IMemoryBlock& memory)
        : IManager(engine, memory),
          _memory(memory),
          _buffers(), // #NewAlloc
          _batches(),
          _currentBuffer(0)
    { }

    SpriteBatchManager::~SpriteBatchManager() noexcept
    {
        for (auto* batch : _batches)
        {
            Memory::Delete(_memory, batch);
        }

        for (auto& buffer : _buffers)
        {
            Memory::Delete(_memory, buffer->_vao);
            Memory::Delete(_memory, buffer->_vbo);
            Memory::Delete(_memory, buffer);
        }
    }

    bool SpriteBatchManager::initialize()
    {
        return true;
    }

    void SpriteBatchManager::clear()
    {
        for (auto* batch : _batches)
        {
            batch->clear();
        }

        for (auto* buffer : _buffers)
        {
            buffer->clear();
        }
    }

    SpriteBuffer& SpriteBatchManager::createNewBuffer(std::uint32_t size)
    {
        _buffers.emplace_back(YAGE_CREATE_NEW(_memory, SpriteBuffer)(_memory, SpriteBuffer::DEFAULT_BUFFER_SIZE));
        _buffers.back()->mapMemory();
        _currentBuffer++;

        return *_buffers.back();
    }

    SpriteBatch& SpriteBatchManager::getSpriteBatch(Utils::Handle<Core::Material> material, Camera* camera, int32_t minimalSize)
    {
        auto* mat = _engine.MaterialManager->tryGetMaterial(material);

        YAGE_ASSERT(camera != nullptr && mat != nullptr, "SpriteBatchManager : Tried to create SpriteBatch without material or camera!");

        BatchIndex index(camera->sortIndex, material.key.index);
        SpriteBatch* batchPtr = nullptr;

        auto batchItr = _batchMap.find(index.key);
        if (batchItr == _batchMap.end())
        {
            _batchMap[index.key] = _batches.size();

            // Todo : dynamically find buffer of best fitting size
            // if (getCurrentBuffer().getFreeSize() < minimalSize)
            // 	createNewBuffer(std::max(minimalSize, SpriteBuffer::MAX_BUFFER_SIZE));

            if (minimalSize < 0)
                minimalSize = SpriteBuffer::DEFAULT_BUFFER_SIZE;

            batchPtr = YAGE_CREATE_NEW(_memory, SpriteBatch)(createNewBuffer(std::min(minimalSize, SpriteBuffer::MAX_BUFFER_SIZE)));
            _batches.push_back(batchPtr);
        }
        else
        {
            batchPtr = _batches[batchItr->second];

            if (minimalSize > 0)
                batchPtr->ensureCapacity(minimalSize);
        }

        YAGE_ASSERT(batchPtr != nullptr, "SpriteBatchManager : was unable to create SpriteBatch!");

        batchPtr->_cameraPtr = camera;
        batchPtr->_materialPtr = mat;

        return *batchPtr;
    }
}
