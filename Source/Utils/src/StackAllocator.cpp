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

	StackAllocator::~StackAllocator()
	{

	}

	void* StackAllocator::allocate(std::size_t size, std::size_t alignment, std::size_t offset)
	{
		std::size_t headerSize = ALLOCATION_SIZE_BYTES + ADJUSTMENT_SIZE_BYTES + offset;
		std::size_t adjustment = Internal::calcForwardAlignmentAdjustment(_currentPtr, alignment, headerSize);

		if(_usedSize + adjustment + size > _size)
			return nullptr;

		std::uintptr_t currentAddress = reinterpret_cast<std::uintptr_t >(_currentPtr);
		std::uintptr_t alignedAddress = currentAddress + adjustment;
		std::uintptr_t adjustmentSize = alignedAddress - headerSize;
		std::uintptr_t allocationSize = adjustmentSize + ADJUSTMENT_SIZE_BYTES;

		_usedSize 		+= size + adjustment;
		_currentPtr 	 = reinterpret_cast<void*>(alignedAddress + size);
		_lastAllocation  = reinterpret_cast<void*>(alignedAddress - offset);

		*(reinterpret_cast<std::size_t*> (allocationSize)) = ((std::size_t)	size + adjustment);
		*(reinterpret_cast<std::uint8_t*>(adjustmentSize)) = ((uint8_t)	adjustment);

		return _lastAllocation;
	}

	std::size_t StackAllocator::getAllocationSize(const void *ptr) const
	{
		std::uintptr_t 	allocationSizeAddress	= reinterpret_cast<std::uintptr_t>(ptr) - ALLOCATION_SIZE_BYTES;
		std::uintptr_t 	adjustmentSizeAddress	= allocationSizeAddress - ADJUSTMENT_SIZE_BYTES;
		std::size_t 	allocationSize			= *reinterpret_cast<std::size_t*> (allocationSizeAddress);
		uint8_t 		adjustmentSize			= *reinterpret_cast<std::uint8_t*>(adjustmentSizeAddress);
		return allocationSize - adjustmentSize;
	}

	void* StackAllocator::resize(void* ptr, std::size_t newSize)
	{
		assert(ptr == _lastAllocation && "Can only resize from the top of a stack");

		std::size_t lastAddress = reinterpret_cast<uintptr_t>(_lastAllocation);
		std::size_t allocSize 	= getAllocationSize(_lastAllocation);
		std::size_t sizeDiff 	= newSize - allocSize;

		assert(sizeDiff > 0);

		_usedSize 	-= sizeDiff;
		_currentPtr  = reinterpret_cast<void*>(lastAddress + sizeDiff);

		return _lastAllocation;
	}

	void StackAllocator::deallocate(void *ptr)
	{
		assert(ptr == _lastAllocation && "Can only deallocate from the top of a stack");

		std::size_t lastAddress 	= reinterpret_cast<uintptr_t>(_lastAllocation);
		std::uintptr_t 	allocationSizeAddress	= reinterpret_cast<std::uintptr_t>(ptr) - ALLOCATION_SIZE_BYTES;
		std::size_t 	allocationSize			= *reinterpret_cast<std::size_t*> (allocationSizeAddress);

		_usedSize -= allocationSize;

		_currentPtr 	= reinterpret_cast<void*>(lastAddress - ALLOCATION_SIZE_BYTES - ADJUSTMENT_SIZE_BYTES);
		_lastAllocation = reinterpret_cast<void*>(lastAddress - allocationSize);
	}

	bool StackAllocator::hasAddress(const void* ptr) const
	{
		return ptr >= getStart() && ptr <= getEnd();
	}
}
