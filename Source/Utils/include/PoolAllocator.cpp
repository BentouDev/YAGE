//
// Created by bentoo on 9/28/16.
//

#include <assert.h>
#include "PoolAllocator.h"

namespace Memory
{
	PoolAllocator::PoolAllocator(void *memory, std::size_t size,
		 std::size_t typeSize, std::size_t typeAlign, std::size_t alignOffset)
		: _typeSize(typeSize), _typeAlignment(typeAlign), _alignOffset(alignOffset), Allocator(memory, size)
	{
		assert(typeSize > sizeof(void*) && "Allocation size must be larger than sizeof(void*)!");

		std::size_t allocSize 	= typeSize + alignOffset;
		std::size_t adjustment 	= Internal::calcForwardAlignmentAdjustment(_startPtr, typeAlign, alignOffset);

		std::uintptr_t 	startAddress	= reinterpret_cast<uintptr_t>(_startPtr);
		std::uintptr_t 	alignedAddress 	= startAddress + adjustment;

		LinkedAddress current;
		current.ptr = reinterpret_cast<void**>(alignedAddress);

		_usedSize 	+= adjustment;
		_freePtr.ptr = current.ptr;

		std::size_t maxObjCount = getFreeSize() / allocSize;

		// build linked list using raw pointers
		for(unsigned i = 0; i < maxObjCount - 1; i++)
		{
			std::uintptr_t address = reinterpret_cast<std::uintptr_t>(current.ptr);
			current.next = reinterpret_cast<void*>(address + allocSize);
			current.ptr	 = (void**) current.next;
		}

		// end linked list
		current.next = nullptr;
	}

	void* PoolAllocator::allocate(std::size_t size, std::size_t alignment, std::size_t offset)
	{
		assert(size == _typeSize && alignment == _typeAlignment
			   && "Cannot do allocation with other size and alignment");

		if(_freePtr.ptr == nullptr)
			return nullptr;

		LinkedAddress newPtr = _freePtr;

		_freePtr.ptr = (void**)(newPtr.next);

		_usedSize += size;

		return newPtr.ptr;
	}

	std::size_t PoolAllocator::getAllocationSize(const void *ptr) const
	{
		return _typeSize + _alignOffset;
	}

	void PoolAllocator::deallocate(void *ptr)
	{
		LinkedAddress oldPtr;
		oldPtr.ptr = (void**) ptr;
		oldPtr.next = _freePtr.ptr;

		_usedSize	-= _typeSize;
		_freePtr	 = oldPtr;
	}

	bool PoolAllocator::hasAddress(const void *ptr) const
	{
		return ptr > getStart() && ptr < getEnd();
	}
}
