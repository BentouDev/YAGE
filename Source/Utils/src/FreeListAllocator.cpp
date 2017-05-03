//
// Created by bentoo on 9/28/16.
//

#include <cstring>
#include "Utils/Assert.h"
#include "Utils/FreeListAllocator.h"

#if YAGE_VALGRIND
#include <valgrind/valgrind.h>
#include <valgrind/memcheck.h>

#endif

namespace Memory
{
	FreeListAllocator::FreeListAllocator(void* memory, std::size_t size)
		: IAllocator(memory, size), _freeBlocks(nullptr)
	{
		_freeBlocks = new (memory) FreeListHeader(size);
#if YAGE_VALGRIND
		VALGRIND_MAKE_MEM_DEFINED(_freeBlocks, sizeof(FreeListHeader));
#endif
	}

	FreeListAllocator::FreeListHeader* FreeListAllocator::findBestFitBlock(std::size_t size, std::size_t alignment, std::size_t offset)
	{
		std::size_t headerSize		= sizeof(FreeListHeader) + offset;
		std::size_t smallestSize	= _size;
		FreeListHeader* ptr			= _freeBlocks;
		FreeListHeader* smallest	= nullptr;

		while(ptr != nullptr)
		{
			std::size_t  adjustment	= Internal::calcForwardAlignmentAdjustment(ptr, alignment, headerSize);
			std::size_t  ptrSize	= ptr->size;
			std::size_t  totalSize	= size + adjustment;
			std::int64_t sizeDiff	= (int64_t)(ptrSize - totalSize);

			if(sizeDiff > 0 && ((size_t) sizeDiff) <= smallestSize)
			{
				smallestSize = (size_t) sizeDiff;
				smallest = ptr;
			}

			ptr = ptr->next;
		}

		return smallest;
	}

	void* FreeListAllocator::allocate(std::size_t size, std::size_t alignment, std::size_t offset)
	{
		// search through available blocks
		// at the begining there will be only one
		// so take the best fitting one (one with blockPtr diff in size)
		// and extract from it needed memory, ergo, create new block in between
		// (set previous as this, and previous-> next = this, this->next oldnext

		std::size_t headerSize		= sizeof(FreeListHeader) + offset;
		FreeListHeader* blockPtr 	= findBestFitBlock(size, alignment, offset);

		if(blockPtr == nullptr)
			return nullptr;

		// Calculate forward adjustment
		std::size_t adjustment		= Internal::calcForwardAlignmentAdjustment(blockPtr, alignment, headerSize);
		std::size_t allocationSize	= size + adjustment;

		std::uintptr_t 	blockAddress		= reinterpret_cast<std::uintptr_t>(blockPtr);
		std::uintptr_t 	alignedAddress		= blockAddress + adjustment;
		std::uintptr_t 	endAddress			= blockAddress + allocationSize;

		void* 			newPtr	= reinterpret_cast<void*>(endAddress);
		FreeListHeader* pNew	= reinterpret_cast<FreeListHeader*>(newPtr);

		YAGE_ASSERT(blockPtr->next != pNew,
					"FreeListAllocator : Deallocation has failed to join adjacent blocks for address '%p'!",
					pNew);

		const uint32_t minimalSize = sizeof(FreeListHeader);
		const std::size_t sizeLeft = blockPtr->size - allocationSize;

		if(sizeLeft > minimalSize)
		{
#if YAGE_VALGRIND
			VALGRIND_MAKE_MEM_DEFINED(pNew, sizeof(FreeListHeader));
#endif
			new (pNew) FreeListHeader(sizeLeft);
		//	pNew->next			= nullptr;
		//	pNew->size			= sizeLeft;
		//	pNew->adjustment	= (uint8_t)0;

			if(blockPtr->next == nullptr)
				blockPtr->next = pNew;

		//	doRemoveFromList(_freeBlocks, blockPtr);
			removeFromList(blockPtr);
		//	FreeListHeader* end = getListEnd(_freeBlocks);
		//	end->next 			= pNew;
		}
		else
		{
			allocationSize = blockPtr->size;
			removeFromList(blockPtr);
		//	doRemoveFromList(_freeBlocks, blockPtr);
		}

		//if(blockPtr == _freeBlocks)
		{
		//	_freeBlocks = blockPtr->next;
		}

		std::memset(blockPtr, 0, sizeof(FreeListHeader));

		FreeListHeader* userHeader = reinterpret_cast<FreeListHeader*>(alignedAddress - offset - sizeof(FreeListHeader));

		userHeader->next		= nullptr;
		userHeader->size		= allocationSize;
		userHeader->adjustment	= (uint8_t)adjustment;

		_usedSize += allocationSize;

		void*  userPtr = reinterpret_cast<void*>(alignedAddress - offset);
		return userPtr;
	}

	void FreeListAllocator::removeFromList(FreeListHeader* element)
	{
		FreeListHeader** ptr = &_freeBlocks;

		while((*ptr) != nullptr && (*ptr) != element)
		{
			ptr = &(*ptr)->next;
		}

		if((*ptr) != nullptr)
		{
			(*ptr) = element->next;
		}
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
			ptr = ptr->next;
		}

		return ptr;
	}

	void FreeListAllocator::doRemoveFromList(FreeListHeader*, FreeListHeader* ptr)
	{
		FreeListHeader* previous = reinterpret_cast<FreeListHeader*>(findPreviousInFreeList(ptr));
		if(previous != nullptr)
		{
			previous->next = ptr->next;
		}
	}

	void FreeListAllocator::deallocate(void *ptr)
	{
		std::size_t 	headerSize		= sizeof(FreeListHeader);
		std::uintptr_t  ptrAddress		= reinterpret_cast<std::uintptr_t>(ptr);
		std::uintptr_t 	headerAddress	= ptrAddress - headerSize;
		void* 			rawBegin		= reinterpret_cast<void*>(headerAddress);
		FreeListHeader* header			= reinterpret_cast<FreeListHeader*>(rawBegin);

		YAGE_ASSERT(header->adjustment != 0,
					"FreeListAllocator : Cannot deallocate address '%p', its already free!",
					header);

		std::size_t allocSize 			= header->size;
		void* 		rawEnd 				= reinterpret_cast<void*>(headerAddress + allocSize);
		void* 		freeBeforeStart		= findRawPreviousInFreeList(rawBegin);
		void* 		freeAfterEnd		= findInFreeList(rawEnd);

		// Set address as free
		header->adjustment = (uint8_t)0;

		if(freeAfterEnd == nullptr && freeBeforeStart == nullptr)
		{
			header->next = _freeBlocks;
			_freeBlocks = header;
		//	FreeListHeader* end = getListEnd(_freeBlocks);
		//	end->next = header;
		}
		else if(freeAfterEnd != nullptr && freeBeforeStart != nullptr)
		{
			FreeListHeader* headerAfter	 = reinterpret_cast<FreeListHeader*>(freeAfterEnd);
			FreeListHeader* beforeHeader = reinterpret_cast<FreeListHeader *>(freeBeforeStart);
			beforeHeader->size += header->size + headerAfter->size;

			//doRemoveFromList(_freeBlocks, headerAfter);
			removeFromList(headerAfter);

			FreeListHeader* previous = reinterpret_cast<FreeListHeader*>(findPreviousInFreeList(headerAfter));
			if(previous != nullptr && previous != beforeHeader)
			{
				previous->next = beforeHeader;
			}

		//	if(headerAfter == _freeBlocks)
		//		_freeBlocks = beforeHeader;
		}
		else if(freeAfterEnd != nullptr)
		{
			FreeListHeader* headerAfter = reinterpret_cast<FreeListHeader*>(freeAfterEnd);
			header->size += headerAfter->size;
			header->next  = headerAfter->next;

		//	removeFromList(headerAfter);

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

	void* FreeListAllocator::findPreviousInFreeList(void* ptr)
	{
		// Check if ptr end is in free list
		FreeListHeader* current = _freeBlocks;
		while(current != nullptr && current->next != ptr)
		{
			current = current->next;
		}

		return current;
	}

	void* FreeListAllocator::findInFreeList(void* ptr)
	{
		// Check if ptr is in free list
		FreeListHeader* current = _freeBlocks;
		while(current != nullptr && current != ptr)
		{
			current = current->next;
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
			current = current->next;
			currentAddress = reinterpret_cast<std::uintptr_t>(current);
		}

		return current;
	}
}
