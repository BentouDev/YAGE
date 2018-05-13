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

		while (ptr != nullptr)
		{
			std::size_t  adjustment	= Internal::calcForwardAlignmentAdjustment(ptr, alignment, headerSize);
			std::size_t  ptrSize	= ptr->size;
			std::size_t  totalSize	= size + adjustment - offset; // adjustment already has frontOffset! size does not!
			std::int64_t sizeDiff	= (int64_t)(ptrSize - totalSize);

			if (sizeDiff >= 0 && ((size_t) sizeDiff) <= smallestSize)
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

		if (blockPtr == nullptr)
			return nullptr;

		// Calculate forward adjustment
		std::size_t adjustment		= Internal::calcForwardAlignmentAdjustment(blockPtr, alignment, headerSize);
		std::size_t allocationSize	= size + adjustment - offset; // (adjustment has fronOffset)

		std::uintptr_t 	blockAddress		= reinterpret_cast<std::uintptr_t>(blockPtr);
		std::uintptr_t 	alignedAddress		= blockAddress + adjustment;
		std::uintptr_t 	endAddress			= blockAddress + allocationSize;

		void* 			newPtr	= reinterpret_cast<void*>(endAddress);
		FreeListHeader* pNew	= reinterpret_cast<FreeListHeader*>(newPtr);

		YAGE_ASSERT (
			blockPtr->size >= allocationSize,
			"FreeListAllocator : Attempt to allocate '%zu' bytes in block of size '%zu'! Original alloc size '%zu'.",
			allocationSize, blockPtr->size, size
		);

		if (blockPtr->next != nullptr)
		{
			YAGE_ASSERT (
				!(blockPtr->next >= blockPtr && reinterpret_cast<char*>(blockPtr->next) <= reinterpret_cast<char*>(blockPtr) + blockPtr->size),
				"FreeListAllocator : Memory blocks are damaged, block '%p' exists inside other from '%p' to '%p'.",
				pNew, blockPtr, blockPtr + blockPtr->size
			);
		}

		YAGE_ASSERT (
			blockPtr->next != pNew,
			"FreeListAllocator : Deallocation has failed to join adjacent blocks for address '%p'!",
			pNew
		);

		const uint32_t minimalSize = sizeof(FreeListHeader);
		std::size_t sizeDifference = blockPtr->size - allocationSize;

		if (sizeDifference > minimalSize)
		{
#if YAGE_VALGRIND
			VALGRIND_MAKE_MEM_DEFINED(pNew, sizeof(FreeListHeader));
#endif
			new (pNew) FreeListHeader(sizeDifference);

			auto* end = getListEnd(_freeBlocks);
			end->next = pNew;

			removeFromList(blockPtr);

			sizeDifference = 0;
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

		userHeader->next			= nullptr;
		userHeader->size			= allocationSize; // ?? adjustment has this offset, with padding and header size
		userHeader->adjustment		= (uint8_t)(adjustment - offset);
		userHeader->sizeDifference	= (uint8_t)(sizeDifference);

		_usedSize += allocationSize;

		void*  userPtr = reinterpret_cast<void*>(alignedAddress - offset);
		return userPtr;
	}

	void FreeListAllocator::removeFromList(FreeListHeader* element)
	{
		FreeListHeader** ptr = &_freeBlocks;

		while ((*ptr) != nullptr && (*ptr) != element)
		{
			ptr = &(*ptr)->next;
		}

		if ((*ptr) != nullptr)
		{
			(*ptr) = element->next;
		}
	}

	std::size_t FreeListAllocator::getAllocationSize(const void* ptr) const
	{
		std::uintptr_t 	headerAddress	= reinterpret_cast<std::uintptr_t>(ptr) - sizeof(FreeListHeader);
		FreeListHeader* header			= reinterpret_cast<FreeListHeader*>(headerAddress);
		return header->size - header->adjustment - header->sizeDifference;
	}

	FreeListAllocator::FreeListHeader* FreeListAllocator::getListEnd(FreeListHeader* list)
	{
		FreeListHeader* ptr = list;
		while (ptr->next != nullptr)
		{
			ptr = ptr->next;
		}

		return ptr;
	}

	void FreeListAllocator::doRemoveFromList(FreeListHeader*, FreeListHeader* ptr)
	{
		FreeListHeader* previous = reinterpret_cast<FreeListHeader*>(findPreviousInFreeList(ptr));
		if (previous != nullptr)
		{
			previous->next = ptr->next;
		}
	}

	void FreeListAllocator::deallocate(void *ptr)
	{
		char*			freedPtr		= reinterpret_cast<char*>(ptr);
		std::size_t 	headerSize		= sizeof(FreeListHeader);
		std::uintptr_t  ptrAddress		= reinterpret_cast<std::uintptr_t>(ptr);
		std::uintptr_t 	headerAddress	= ptrAddress - headerSize;
		FreeListHeader* header			= reinterpret_cast<FreeListHeader*>(headerAddress);
		void* 			rawBegin		= reinterpret_cast<void*>(freedPtr - header->adjustment);

		YAGE_ASSERT (
			header->adjustment != 0,
			"FreeListAllocator : Cannot deallocate address '%p', its already free!",
			header
		);

		std::size_t allocSize 			= header->size;
		void* 		rawEnd 				= reinterpret_cast<void*>(reinterpret_cast<char*>(rawBegin) + allocSize);
		void* 		freeBeforeStart		= findRawPreviousInFreeList(rawBegin);
		void* 		freeAfterEnd		= findInFreeList(rawEnd);

		// Copy header data
		FreeListHeader freeHeader(*header);

		// Set address as free
		freeHeader.adjustment = (uint8_t)0;

		// Set data to begin of block
		*reinterpret_cast<FreeListHeader*>(rawBegin) = freeHeader;

		// Set new header ptr back to header variable
		header = reinterpret_cast<FreeListHeader*>(rawBegin);

		if (freeAfterEnd == nullptr && freeBeforeStart == nullptr)
		{
			// Just a new block

			header->next = _freeBlocks;
			_freeBlocks  = header;
		//	FreeListHeader* end = getListEnd(_freeBlocks);
		//	end->next = header;
		}
		else if (freeAfterEnd != nullptr && freeBeforeStart != nullptr)
		{
			// Can be merged with both following and previous blocks

			FreeListHeader* headerAfter	 = reinterpret_cast<FreeListHeader*>(freeAfterEnd);
			FreeListHeader* beforeHeader = reinterpret_cast<FreeListHeader *>(freeBeforeStart);
			beforeHeader->size += header->size + headerAfter->size;

			//doRemoveFromList(_freeBlocks, headerAfter);
			removeFromList(headerAfter);

			FreeListHeader* previous = reinterpret_cast<FreeListHeader*>(findPreviousInFreeList(headerAfter));
			if (previous != nullptr && previous != beforeHeader)
			{
				previous->next = beforeHeader;
			}

		//	if(headerAfter == _freeBlocks)
		//		_freeBlocks = beforeHeader;
		}
		else if(freeAfterEnd != nullptr)
		{
			// Can be merged with following block

			FreeListHeader* headerAfter = reinterpret_cast<FreeListHeader*>(freeAfterEnd);
			header->size += headerAfter->size;
			header->next  = headerAfter->next;

		//	removeFromList(headerAfter);

			FreeListHeader* previous = reinterpret_cast<FreeListHeader*>(findPreviousInFreeList(headerAfter));
			if (previous != nullptr)
			{
				previous->next = header;
			}

			if (headerAfter == _freeBlocks)
				_freeBlocks = header;
		}
		else if (freeBeforeStart != nullptr)
		{
			// Can be merged to previous free block

			FreeListHeader *beforeHeader = reinterpret_cast<FreeListHeader *>(freeBeforeStart);

			beforeHeader->size += header->size;

			header->size = 0;
			header->next = nullptr;
		}

		YAGE_ASSERT (
			!(header->next >= header && reinterpret_cast<char*>(header->next) <= reinterpret_cast<char*>(header) + header->size),
			"FreeListAllocator : Deallocation failed to join adjacent blocks, '%p' is inside other block from '%p' to '%p'.",
			header->next, header, header + header->size
		);

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
		while (current != nullptr && current->next != ptr)
		{
			current = current->next;
		}

		return current;
	}

	void* FreeListAllocator::findInFreeList(void* ptr)
	{
		// Check if ptr is in free list
		FreeListHeader* current = _freeBlocks;
		while (current != nullptr && current != ptr)
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
		while (current != nullptr && (currentAddress + current->size) != ptrAddress)
		{
			current = current->next;
			currentAddress = reinterpret_cast<std::uintptr_t>(current);
		}

		return current;
	}
}
