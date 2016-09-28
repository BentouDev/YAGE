//
// Created by bentoo on 9/27/16.
//

#include <cassert>
#include "LinearAllocator.h"

namespace Memory
{
	LinearAllocator::LinearAllocator(void* memory, std::size_t size)
		: _currentPtr(memory), Allocator(memory, size)
	{

	}

	void LinearAllocator::clear()
	{
		_usedSize = 0;
		_currentPtr = _startPtr;
	}

	void* LinearAllocator::allocate(std::size_t size, std::size_t alignment, std::size_t offset)
	{
		std::size_t adjustment = Internal::calcForwardAlignmentAdjustment(_currentPtr, alignment, offset);

		if(_usedSize + adjustment + size > _size)
			return nullptr;

		std::uintptr_t currentAddress = reinterpret_cast<std::uintptr_t >(_currentPtr);
		std::uintptr_t alignedAddress = currentAddress + adjustment;

		_usedSize 	+= size + adjustment;
		_currentPtr  = reinterpret_cast<void*>(alignedAddress + size);

		return reinterpret_cast<void*>(alignedAddress);
	}

	std::size_t LinearAllocator::getAllocationSize(const void* ptr) const
	{
		return 0;
	}

	void LinearAllocator::deallocate(void* ptr)
	{
		assert(false && "Cannot deallocate from linear allocator, use clear() instead!");
	}

	bool LinearAllocator::hasAddress(const void* ptr) const
	{
		return ptr >= getStart() && ptr < getEnd();
	}
}