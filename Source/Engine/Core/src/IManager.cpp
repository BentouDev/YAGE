//
// Created by bentoo on 14.11.16.
//

#include <Platform/Logger.h>
#include "Core/IManager.h"
#include "Core/Engine.h"

namespace Core
{
	IManager::IManager(Core::Engine &engine, Memory::IMemoryBlock& memory)
		: _engine(engine), _memory(memory)
	{

	}

	IManager::~IManager() noexcept
	{

	}
}
