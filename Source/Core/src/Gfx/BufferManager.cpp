//
// Created by bentoo on 11/3/16.
//

#include "Core/Engine.h"
#include "Core/Gfx/BufferManager.h"

namespace Gfx
{
	BufferManager::BufferManager(Core::Engine &engine, Memory::IMemoryBlock &memory)
		: IManager(engine, memory), _bufferContainer(_memory)
	{

	}

	BufferManager::~BufferManager()
	{

	}
}
