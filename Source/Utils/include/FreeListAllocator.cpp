//
// Created by bentoo on 9/28/16.
//

#include "FreeListAllocator.h"

namespace Memory
{
	FreeListAllocator::FreeListAllocator(void *memory, size_t size)
		: Allocator(memory, size)
	{

	}
}

