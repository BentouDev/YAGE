//
// Created by bentoo on 9/28/16.
//

#include <cstring>
#include <assert.h>
#include "Utils/FreeListAllocator.h"

namespace Memory
{
	FreeListAllocator::FreeListAllocator(void *memory, std::size_t size)
		: _freeBlocks((FreeListHeader*) memory), IAllocator(memory, size)
	{
		_freeBlocks->next 		= nullptr;
		_freeBlocks->size 		= (uint32_t )size;
		_freeBlocks->adjustment = (uint8_t)0;
	}

	void* FreeListAllocator::allocate(std::size_t size, std::size_t alignment, std::size_t offset)
	{
		// search through available blocks
		// at the begining there will be only one
		// so take the best fitting one (one with smallest diff in size)
		// and extract from it needed memory, ergo, create new block in between
		// (set previous as this, and previous-> next = this, this->next oldnext

		std::size_t headerSize 		= sizeof(FreeListHeader) + offset;
		std::size_t smallestSize 	= _size;

		FreeListHeader* ptr		 = _freeBlocks;
		FreeListHeader* smallest = nullptr;

		while(ptr != nullptr)
		{
			std::size_t  adjustment = Internal::calcForwardAlignmentAdjustment(ptr, alignment, headerSize);
			std::int32_t sizeDiff 	= ptr->size - (uint32_t)(size + adjustment);

			if(sizeDiff > 0 && sizeDiff <= smallestSize)
			{
				smallestSize = sizeDiff;
				smallest = ptr;
			}

			ptr = reinterpret_cast<FreeListHeader*>(ptr->next);
		}

		if(smallest == nullptr)
			return nullptr;

		// Calculate forward adjustment
		std::size_t adjustment 		= Internal::calcForwardAlignmentAdjustment(smallest, alignment, headerSize);
		std::size_t allocationSize 	= size + adjustment;

		std::uintptr_t 	smallestAddress 	= reinterpret_cast<std::uintptr_t>(smallest);
		// Aligned address start
		std::uintptr_t 	alignedAddress		= smallestAddress + adjustment;
		// Address of block end
		std::uintptr_t 	endAddress 			= smallestAddress + allocationSize;

		void* 			newPtr 	= reinterpret_cast<void*>(endAddress);
		FreeListHeader* pNew 	= reinterpret_cast<FreeListHeader*>(newPtr);

		assert(smallest->next != pNew && "Deallocation has failed to join adjacent blocks!");

		const uint32_t minimalSize = sizeof(FreeListHeader);
		const uint32_t sizeLeft = smallest->size - allocationSize;

		if(sizeLeft > minimalSize)
		{
			// TODO: fix baka mistake!
			// WTF is that :D ?
			// We shouldn't do this!
			// We should check how big will remaining space be
			// If its bigger than header, do as we do now
			// But if its not, treat all this block as allocated!
			pNew->next		 	= nullptr;
			pNew->size		 	= sizeLeft;
			pNew->adjustment 	= (uint8_t)0;

			// we may use ptr, as it already points to smallest
			doRemoveFromList(_freeBlocks, smallest);
			FreeListHeader* end = getListEnd(_freeBlocks);
			end->next 			= pNew;
		}
		else
		{
			allocationSize = smallest->size;
			doRemoveFromList(_freeBlocks, smallest);
		}

		if(smallest == _freeBlocks)
		{
			_freeBlocks = reinterpret_cast<FreeListHeader*>(smallest->next);
		}

	//	smallest->next		 = nullptr;
	//	smallest->size 		 = (uint32_t)allocationSize;
	//	smallest->adjustment = (uint8_t)adjustment;

		std::memset(smallest, 0, adjustment);

		FreeListHeader* userHeader = reinterpret_cast<FreeListHeader*>(alignedAddress - offset - sizeof(FreeListHeader));

		userHeader->next		= nullptr;
		userHeader->size		= (uint32_t)allocationSize;
		userHeader->adjustment	= (uint8_t)adjustment;

		_usedSize += allocationSize;

		void*  userPtr = reinterpret_cast<void*>(alignedAddress - offset);
		return userPtr;
	}

	std::size_t FreeListAllocator::getAllocationSize(const void* ptr) const
	{
		std::uintptr_t 	headerAddress	= reinterpret_cast<std::uintptr_t>(ptr) - sizeof(FreeListHeader);
		FreeListHeader* header			= reinterpret_cast<FreeListHeader*>(headerAddress);
		return header->size - header->adjustment;
	}

	FreeListAllocator::FreeListHeader* FreeListAllocator::getListEnd(FreeListHeader* list)
	{
		FreeListHeader* ptr = list;
		while(ptr->next != nullptr)
		{
			ptr = reinterpret_cast<FreeListHeader*>(ptr->next);
		}

		return ptr;
	}

	void FreeListAllocator::doRemoveFromList(FreeListHeader* list, FreeListHeader* ptr)
	{
		FreeListHeader* previous = reinterpret_cast<FreeListHeader*>(findPreviousInFreeList(ptr));
		if(previous != nullptr)
		{
			previous->next = ptr->next;
		}
	}

	void FreeListAllocator::deallocate(void *ptr)
	{
		std::size_t 	headerSize	= sizeof(FreeListHeader);
		std::uintptr_t  ptrAddress	= reinterpret_cast<std::uintptr_t>(ptr);
		void* 			headerBegin	= reinterpret_cast<void*>(ptrAddress - headerSize);

		FreeListHeader* header		= reinterpret_cast<FreeListHeader*>(headerBegin);
		std::uintptr_t	rawAddress	= ptrAddress - header->adjustment;
		void*			rawBegin	= reinterpret_cast<void*>(headerBegin); //rawAddress

		assert(header->adjustment != 0 && "Cannot deallocate address thats already free!");

		std::size_t allocSize 			= header->size;
	//	std::size_t originalAllocSize 	= allocSize - headerSize;
		void* 		rawEnd 				= reinterpret_cast<void*>(headerBegin + allocSize); // rawAddress
		void* 		freeBeforeStart		= findRawPreviousInFreeList(rawBegin);
		void* 		freeAfterEnd		= findInFreeList(rawEnd);

		// Set address as free
		header->adjustment = (uint8_t)0;

		if(freeAfterEnd == nullptr && freeBeforeStart == nullptr)
		{
			FreeListHeader* end = getListEnd(_freeBlocks);
			end->next = header;
		}
		else if(freeAfterEnd != nullptr && freeBeforeStart != nullptr)
		{
			FreeListHeader* headerAfter	 = reinterpret_cast<FreeListHeader*>(freeAfterEnd);
			FreeListHeader* beforeHeader = reinterpret_cast<FreeListHeader *>(freeBeforeStart);
			beforeHeader->size += header->size + headerAfter->size;

			doRemoveFromList(_freeBlocks, headerAfter);

			FreeListHeader* previous = reinterpret_cast<FreeListHeader*>(findPreviousInFreeList(headerAfter));
			if(previous != nullptr && previous != beforeHeader)
			{
				previous->next = beforeHeader;
			}

			if(headerAfter == _freeBlocks)
				_freeBlocks = beforeHeader;
		}
		else if(freeAfterEnd != nullptr)
		{
			FreeListHeader* headerAfter = reinterpret_cast<FreeListHeader*>(freeAfterEnd);
			header->size += headerAfter->size;
			header->next  = headerAfter->next;

			FreeListHeader* previous = reinterpret_cast<FreeListHeader*>(findPreviousInFreeList(headerAfter));
			if(previous != nullptr)
			{
				previous->next = header;
			}

			if(headerAfter == _freeBlocks)
				_freeBlocks = header;
		}
		else if(freeBeforeStart != nullptr)
		{
			FreeListHeader *beforeHeader = reinterpret_cast<FreeListHeader *>(freeBeforeStart);

			beforeHeader->size += header->size;

			header->size = 0;
			header->next = nullptr;
		}

		_usedSize -= allocSize;
	}

	bool FreeListAllocator::hasAddress(const void *ptr) const
	{
		return ptr >= getStart() && ptr <= getEnd();
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
}
