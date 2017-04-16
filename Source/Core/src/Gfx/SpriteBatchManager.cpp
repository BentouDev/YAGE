//
// Created by Bentoo on 2017-04-13.
//

#include "Core/Gfx/SpriteBatchManager.h"
#include "Core/Gfx/SpriteBatch.h"
#include "Core/Gfx/Sprite.h"
#include "Core/Gfx/Camera.h"
#include "Core/Resources/Material/MaterialManager.h"
#include "Core/Engine.h"
#include "Core/Logger.h"

namespace Gfx
{
	const std::int32_t SpriteBuffer::DEFAULT_BUFFER_SIZE = 1000;

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
		gl::VertexAttribPointer(0, 2, gl::FLOAT, gl::FALSE_, sizeof(SpriteVertex), reinterpret_cast<void*>(0));
		OpenGL::checkError();

		gl::EnableVertexAttribArray(1);
		OpenGL::checkError();
		gl::VertexAttribPointer(1, 2, gl::FLOAT, gl::FALSE_, sizeof(SpriteVertex), reinterpret_cast<void*>(sizeof(glm::vec2)));
		OpenGL::checkError();

		gl::EnableVertexAttribArray(2);
		OpenGL::checkError();
		gl::VertexAttribPointer(2, 4, gl::UNSIGNED_BYTE, gl::TRUE_, sizeof(SpriteVertex), reinterpret_cast<void*>(2 * sizeof(glm::vec2)));
		OpenGL::checkError();

		Core::Logger::info("SpriteBatchManager : Created sprite VAO!");

		gl::BindVertexArray(0);
		gl::BindBuffer(gl::ARRAY_BUFFER, 0);
	}

	void SpriteBuffer::clear()
	{
		currentSize = 0;
		unmapMemory();
		mapMemory();
	}

	void SpriteBuffer::mapMemory()
	{
		if(_mappedPtr != nullptr)
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
		if(_mappedPtr == nullptr)
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
		mempcpy(buffer, array, sizeof(SpriteVertex) * 6);

		buffer		+= 6;
		currentSize	+= 6;
		_mappedPtr	 = buffer;
	}

	SpriteBatchManager::SpriteBatchManager(Core::Engine& engine,
										   Memory::IMemoryBlock& memory)
		: IManager(engine, memory),
		  _memory(memory),
		  _buffers(memory),
		  _batches(memory),
		  _currentBuffer(0)
	{ }

	SpriteBatchManager::~SpriteBatchManager()
	{
		for(auto& buffer : _buffers)
		{
			Memory::Delete(_memory, buffer._vao);
			Memory::Delete(_memory, buffer._vbo);
		}
	}

	bool SpriteBatchManager::initialize()
	{
		_buffers.emplace(_memory, SpriteBuffer::DEFAULT_BUFFER_SIZE);
		_buffers.back().mapMemory();

		return true;
	}

	void SpriteBatchManager::clear()
	{
		for(auto& batch : _batches)
		{
			batch.clear();
		}

		for(auto& buffer : _buffers)
		{
			buffer.clear();
		}
	}

	SpriteBuffer& SpriteBatchManager::createNewBuffer(std::uint32_t size)
	{
		_buffers.emplace(_memory, size);
		_buffers.back().mapMemory();
		_currentBuffer++;
	}

	SpriteBatch& SpriteBatchManager::getSpriteBatch(Utils::Handle<Core::Material> material, Camera* camera, int32_t minimalSize)
	{
		auto* mat = _engine.MaterialManager->tryGetMaterial(material);

		YAGE_ASSERT(camera != nullptr && mat != nullptr, "SpriteBatchManager : Tried to create SpriteBatch without material or camera!");

		BatchIndex index(camera->sortIndex, material.index);
		SpriteBatch* batchPtr = nullptr;

		auto batchItr = _batchMap.find(index);
		if (batchItr == _batchMap.end())
		{
			_batchMap[index] = _batches.size();

			// Todo : dynamically find buffer of best fitting size
			if (getCurrentBuffer().getFreeSize() < minimalSize)
				createNewBuffer(std::max(minimalSize, SpriteBuffer::DEFAULT_BUFFER_SIZE));

			_batches.emplace(getCurrentBuffer());
			batchPtr = &_batches.back();
		}
		else
		{
			batchPtr = &_batches[batchItr->second];

			if(minimalSize > 0)
				batchPtr->ensureCapacity(minimalSize);
		}

		YAGE_ASSERT(batchPtr != nullptr, "SpriteBatchManager : was unable to create SpriteBatch!");

		batchPtr->_cameraPtr = camera;
		batchPtr->_materialPtr = mat;

		return *batchPtr;
	}
}
