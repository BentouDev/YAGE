//
// Created by bentoo on 9/28/16.
//

#ifndef GAME_FREELISTALLOCATOR_H
#define GAME_FREELISTALLOCATOR_H

#include "Allocator.h"

namespace Memory
{
	class FreeListAllocator : public IAllocator
	{
	private:
		struct FreeListHeader
		{
			explicit FreeListHeader(std::size_t size)
				: next(nullptr)
				, size(size)
				, adjustment(0)
			{ }

			explicit FreeListHeader(const FreeListHeader& other)
				: next(other.next)
				, size(other.size)
				, adjustment(other.adjustment)
			{ }

			FreeListHeader* next;
			std::size_t 	size;
			uint8_t  		adjustment;
			uint8_t 		sizeDifference;
		};

		FreeListAllocator(const FreeListAllocator&) = delete;
		FreeListAllocator(FreeListAllocator&&) = delete;

		FreeListHeader* _freeBlocks;

		void removeFromList(FreeListHeader* element);

		void 			doRemoveFromList(FreeListHeader* list, FreeListHeader* ptr);
		FreeListHeader* getListEnd(FreeListHeader* list);

		FreeListHeader* findBestFitBlock(std::size_t size, std::size_t alignment, std::size_t offset);
		void* findPreviousInFreeList(void* ptr);
		void* findRawPreviousInFreeList(void* ptr);
		void* findInFreeList(void* ptr);

	public:
		FreeListAllocator(void* memory, std::size_t size);

		void* 		allocate(std::size_t size, std::size_t alignment, std::size_t offset) override;
		void 		deallocate(void* ptr) override;
		std::size_t getAllocationSize(const void* ptr) const override;
		bool 		hasAddress(const void* ptr) const override;
	};
}

#endif //GAME_FREELISTALLOCATOR_H
