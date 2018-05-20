//
// Created by bentoo on 14.11.16.
//

#include "Utils/MemoryBoundChecker.h"
#include "Utils/MemoryBlock.h"

namespace Memory
{
	void* IMemoryBoundChecker::getSuperblockPtr() const
	{
		return _block.getSuperblockPtr();
	}

	const char* IMemoryBoundChecker::getName() const
	{
		return _block.getName();
	}
}
