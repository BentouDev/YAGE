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

	void* FreeListAllocator::allocate(std::size_t size, std::size_t alignment, std::size_t offset)
	{

	}

	std::size_t FreeListAllocator::getAllocationSize(const void *ptr) const
	{

	}

	void FreeListAllocator::deallocate(void *ptr)
	{

	}

	bool FreeListAllocator::hasAddress(const void *ptr) const
	{

	}
}

