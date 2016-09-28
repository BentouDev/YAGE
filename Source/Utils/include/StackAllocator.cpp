//
// Created by bentoo on 9/28/16.
//

#include <cassert>
#include "StackAllocator.h"

namespace Memory
{
	StackAllocator::StackAllocator(void* memory, std::size_t size)
		: _currentPtr(memory), _lastAllocation(nullptr), Allocator(memory, size)
	{

	}

	void* StackAllocator::allocate(std::size_t size, std::size_t alignment, std::size_t offset)
	{
		std::size_t headerSize = ALLOCATION_SIZE + offset;
		std::size_t adjustment = Internal::calcForwardAlignmentAdjustment(_currentPtr, alignment, headerSize);

		if(_usedSize + adjustment + size > _size)
			return nullptr;

		std::uintptr_t currentAddress = reinterpret_cast<std::uintptr_t >(_currentPtr);
		std::uintptr_t alignedAddress = currentAddress + adjustment;
		std::uintptr_t headerAddress  = alignedAddress - headerSize;

		_usedSize 		+= size + adjustment;
		_currentPtr 	 = reinterpret_cast<void*>(alignedAddress + size);
		_lastAllocation  = reinterpret_cast<void*>(alignedAddress);

		*(reinterpret_cast<std::size_t*>(headerAddress)) = size + adjustment;

		return reinterpret_cast<void*>(alignedAddress);
	}

	std::size_t StackAllocator::getAllocationSize(const void *ptr) const
	{
		std::uintptr_t headerAddress = reinterpret_cast<std::uintptr_t>(ptr) - ALLOCATION_SIZE;
		return *reinterpret_cast<std::size_t*>(headerAddress);
	}

	void StackAllocator::deallocate(void *ptr)
	{
		assert(ptr == _lastAllocation && "Can only deallocate from the top of a stack");

		std::size_t lastAddress = reinterpret_cast<uintptr_t>(_lastAllocation);
		std::size_t allocSize 	= getAllocationSize(_lastAllocation);

		_usedSize -= allocSize;

		_currentPtr 	= _lastAllocation;
		_lastAllocation = reinterpret_cast<void*>(lastAddress - allocSize);
	}

	bool StackAllocator::hasAddress(const void* ptr) const
	{
		return ptr >= getStart() && ptr <= _lastAllocation;
	}
}