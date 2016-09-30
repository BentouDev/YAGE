//
// Created by bentoo on 9/28/16.
//

#include <assert.h>
#include "FreeListAllocator.h"

namespace Memory
{
	FreeListAllocator::FreeListAllocator(void *memory, std::size_t size)
		: _freeBlocks((FreeListHeader*) memory), Allocator(memory, size)
	{
		_allocatedBlocks	= nullptr;
		_freeBlocks->next 	= nullptr;
		_freeBlocks->size 	= size;
	}

	void* FreeListAllocator::allocate(std::size_t size, std::size_t alignment, std::size_t offset)
	{
		// search through available blocks
		// at the begining there will be only one
		// so take the best fitting one (one with smallest diff in size)
		// and extract from it needed memory, ergo, create new block in between
		// (set previous as this, and previous-> next = this, this->next oldnext

		std::size_t headerSize = sizeof(FreeListHeader) + offset;
		std::size_t smallestSize = _size;

		FreeListHeader* ptr = _freeBlocks;
		FreeListHeader* smallest = nullptr;

		while(ptr != nullptr)
		{
			std::size_t adjustment 	= Internal::calcForwardAlignmentAdjustment(ptr, alignment, headerSize);
			std::size_t sizeDiff 	= ptr->size - (size + adjustment);

			if(sizeDiff > 0 && sizeDiff <= smallestSize)
			{
				smallestSize = sizeDiff;
				smallest = ptr;
			}

			ptr = (FreeListHeader*)ptr->next;
		}

		if(smallest == nullptr)
			return nullptr;

		std::size_t adjustment 		= Internal::calcForwardAlignmentAdjustment(smallest, alignment, headerSize);
		std::size_t allocationSize 	= size + adjustment;

		std::uintptr_t 	smallestAddress = reinterpret_cast<std::uintptr_t>(smallest);
		std::uintptr_t 	currentAddress	= smallestAddress + adjustment;
		std::uintptr_t 	nextAddress 	= smallestAddress + allocationSize;

		void* 			newPtr 	= reinterpret_cast<void*>(nextAddress);
		FreeListHeader* pNew 	= reinterpret_cast<FreeListHeader*>(newPtr);

	//	assert(smallest->next != pNew && "Deallocation has failed to join adjacent blocks!");

		pNew->next 	= smallest->next;
		pNew->size 	= smallest->size - allocationSize;

		// Remove from free list
		removeFromFreeList(smallest, pNew);

		void* userPtr = reinterpret_cast<void*>(currentAddress - offset);
		_usedSize 	 += allocationSize;

		// Add new allocation to list
		{
			AllocatedListHeader* newAlloc = reinterpret_cast<AllocatedListHeader*>(smallest);
			newAlloc->size 		= allocationSize;
			newAlloc->previous 	= _allocatedBlocks;
			_allocatedBlocks 	= newAlloc;
		}

		return userPtr;
	}

	std::size_t FreeListAllocator::getAllocationSize(const void *ptr) const
	{
		std::uintptr_t headerAddress = reinterpret_cast<std::uintptr_t>(ptr) - sizeof(FreeListHeader);
		return reinterpret_cast<FreeListHeader*>(headerAddress)->size;
	}

	void FreeListAllocator::deallocate(void *ptr)
	{
		std::uintptr_t 	ptrAddress 	= reinterpret_cast<std::uintptr_t>(ptr);
		void* 			rawBegin	= reinterpret_cast<void*>(ptrAddress - sizeof(AllocatedListHeader));

		AllocatedListHeader* asAllocated = reinterpret_cast<AllocatedListHeader*>(findInAllocatedList(rawBegin));
		FreeListHeader* 	 asFree 	 = reinterpret_cast<FreeListHeader*>(asAllocated);

		assert(asAllocated == rawBegin && "Cannot deallocate address thats not in allocaton list!");

		std::size_t allocSize 			= asAllocated->size;
		std::size_t originalAllocSize 	= allocSize - sizeof(AllocatedListHeader);
		void* 		rawEnd 				= reinterpret_cast<void*>(ptrAddress + originalAllocSize);

		// Remove from alloc list
		removeFromAllocatedList(asAllocated);

		// Check for adresses that ends on ours begin
		void* freeBeforeStart = findRawPreviousInFreeList(rawBegin);
		// and starts with our end in free list
		void* freeAfterEnd = findInFreeList(rawEnd);

		if(freeAfterEnd != nullptr)
		{
			FreeListHeader* oldHeader = reinterpret_cast<FreeListHeader*>(freeAfterEnd);
			asFree->size += oldHeader->size;

			void* previousPtr = findPreviousInFreeList(freeAfterEnd);
			if(previousPtr != nullptr)
			{
				FreeListHeader* previous = reinterpret_cast<FreeListHeader*>(previousPtr);
				previous->next = asFree;
			}

			asFree->next = oldHeader->next;

			if(_freeBlocks == freeAfterEnd)
			{
				_freeBlocks = asFree;
			}
		}

		if(freeBeforeStart != nullptr)
		{
			std::uintptr_t beginAddress = reinterpret_cast<std::uintptr_t>(rawBegin);
			FreeListHeader* previous = reinterpret_cast<FreeListHeader*>(freeBeforeStart);

			previous->size += asFree->size;

			if(reinterpret_cast<std::uintptr_t>(previous->next) < beginAddress + asFree->size)
			{
		//		assert(freeAfterEnd != nullptr && "Just checking!");
				previous->next = asFree->next;
			}

			asFree->size = 0;
			asFree->next = nullptr;

		//	assert(reinterpret_cast<std::uintptr_t>(previous) + previous->size
		//		   != reinterpret_cast<std::uintptr_t>(previous->next));
		}

		if(freeAfterEnd == nullptr && freeBeforeStart == nullptr)
		{
			asFree->next = _freeBlocks;
			_freeBlocks = asFree;
		}

		_usedSize -= allocSize;
	}

	bool FreeListAllocator::hasAddress(const void *ptr) const
	{
		return ptr >= getStart() && ptr <= getEnd();
	}

	void FreeListAllocator::removeFromFreeList(FreeListHeader* oldFreeBlock, FreeListHeader* newFreeBlock)
	{
		if(_freeBlocks == oldFreeBlock)
		{
			_freeBlocks = newFreeBlock;
		}
		else
		{
			FreeListHeader* previous = reinterpret_cast<FreeListHeader*>(findRawPreviousInFreeList(oldFreeBlock));
			if(previous != nullptr)
			{
				previous->next = newFreeBlock;
			}
		}
	}

	void FreeListAllocator::removeFromAllocatedList(AllocatedListHeader* allocatedBlock)
	{
		if(_allocatedBlocks == allocatedBlock)
		{
			_allocatedBlocks = reinterpret_cast<AllocatedListHeader*>(_allocatedBlocks->previous);
		}
		else
		{
			AllocatedListHeader* next = reinterpret_cast<AllocatedListHeader*>(findNextInAllocatedList(allocatedBlock));
			if(next != nullptr)
			{
				next->previous = allocatedBlock->previous;
			}
		}
	}

	void* FreeListAllocator::findPreviousInFreeList(void *ptr)
	{
		// Check if ptr end is in free list
		FreeListHeader* current = _freeBlocks;
		while(current != nullptr && current->next != ptr)
		{
			current = reinterpret_cast<FreeListHeader*>(current->next);
		}

		return current;
	}

	void* FreeListAllocator::findRawPreviousInFreeList(void *ptr)
	{
		// Check if ptr end is in free list
		FreeListHeader* current 		= _freeBlocks;
		std::uintptr_t  ptrAddress 		= reinterpret_cast<std::uintptr_t>(ptr);
		std::uintptr_t 	currentAddress 	= reinterpret_cast<std::uintptr_t>(current);
		while(current != nullptr && (currentAddress + current->size) != ptrAddress)
		{
			current = reinterpret_cast<FreeListHeader*>(current->next);
			currentAddress = reinterpret_cast<std::uintptr_t>(current);
		}

		return current;
	}

	void* FreeListAllocator::findNextInAllocatedList(void *ptr)
	{
		// Check if ptr is in allocated list
		AllocatedListHeader* current = _allocatedBlocks;
		while(current != nullptr && current->previous != ptr)
		{
			current = reinterpret_cast<AllocatedListHeader*>(current->previous);
		}

		return current;
	}

	void* FreeListAllocator::findInFreeList(void* ptr)
	{
		// Check if ptr is in free list
		FreeListHeader* current = _freeBlocks;
		while(current != nullptr && current != ptr)
		{
			current = reinterpret_cast<FreeListHeader*>(current->next);
		}

		return current;
	}

	void* FreeListAllocator::findInAllocatedList(void* ptr)
	{
		// Check if ptr is in allocated list
		AllocatedListHeader* current = _allocatedBlocks;
		while(current != ptr && current != nullptr)
		{
			current = reinterpret_cast<AllocatedListHeader*>(current->previous);
		}

		return current;
	}
}
