//
// Created by bentoo on 11/3/16.
//

#ifndef GAME_BUFFERMANAGER_H
#define GAME_BUFFERMANAGER_H

#include <Utils/Container.h>
#include "StaticBuffer.h"
#include "Core/IManager.h"

namespace Core
{
	class Engine;
}

namespace Gfx
{
	class BufferManager : public Core::IManager
	{
		MANAGER(BufferManager);

	public:
		using handle_t = StaticBuffer::handle_t;

	protected:
		Utils::Container<StaticBufferTrait>	_bufferContainer;

	public:
		explicit BufferManager(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~BufferManager() noexcept;

		StaticBuffer* tryGetBuffer(handle_t handle);

		handle_t createNew(std::size_t vertexSize, std::size_t indexSize, std::size_t vertexCount, std::size_t indexCount, Core::Context* context = nullptr)
		{ return _bufferContainer.create(_memory, vertexSize, indexSize, vertexCount, indexCount, context); }

		inline StaticBuffer& getBuffer(handle_t handle)
		{ return _bufferContainer.get(handle); }

		inline bool contains(handle_t handle) const noexcept
		{ return _bufferContainer.contains(handle); }
	};
}

#endif //GAME_BUFFERMANAGER_H
