//
// Created by bentoo on 12.11.16.
//

#include "Utils/MemoryTracker.h"
#include "Utils/MemoryBlock.h"

namespace Memory
{
	void* IMemoryTracker::getSuperblockPtr() const
	{
		return _block.getSuperblockPtr();
	}

	const char* IMemoryTracker::getName() const
	{
		return _block.getName();
	}
}
