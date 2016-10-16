//
// Created by bentoo on 9/28/16.
//

#include <assert.h>
#include "Utils/PoolAllocator.h"

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

		void** current = reinterpret_cast<void**>(alignedAddress);

		_usedSize 	+= adjustment;
		_freePtr 	 = current;

		std::size_t maxObjCount = getFreeSize() / allocSize;

		// build linked list using raw pointers
		for(unsigned i = 0; i < maxObjCount - 1; i++)
		{
			std::uintptr_t address = reinterpret_cast<std::uintptr_t>(current);
			*current = reinterpret_cast<void*>(address + allocSize);
			current	 = (void**) *current;
		}

		// end linked list
		*current = nullptr;
	}

	void* PoolAllocator::allocate(std::size_t size, std::size_t alignment, std::size_t offset)
	{
		assert(size == _typeSize && alignment == _typeAlignment
			   && "Cannot do allocation with other size and alignment");

		if(_freePtr == nullptr)
			return nullptr;

		void** newPtr = _freePtr;

		_freePtr	 = (void**)(*newPtr);
		_usedSize	+= size;

		return newPtr;
	}

	std::size_t PoolAllocator::getAllocationSize(const void *ptr) const
	{
		return _typeSize + _alignOffset;
	}

	void PoolAllocator::deallocate(void *ptr)
	{
		void** oldPtr = (void**) ptr;

		*oldPtr = _freePtr;

		_usedSize	-= _typeSize;
		_freePtr	 = oldPtr;
	}

	bool PoolAllocator::hasAddress(const void *ptr) const
	{
		return ptr > getStart() && ptr < getEnd();
	}
}
