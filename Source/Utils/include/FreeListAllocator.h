//
// Created by bentoo on 9/28/16.
//

#ifndef GAME_FREELISTALLOCATOR_H
#define GAME_FREELISTALLOCATOR_H

#include "Allocator.h"

namespace Memory
{
	class FreeListAllocator : public Allocator
	{
	public:

		struct FreeListHeader
		{
			void* 	 next;
			uint32_t size;
			uint8_t  adjustment;
		};

		struct AllocatedListHeader
		{
			void* 	 previous;
			uint32_t size;
			uint8_t	 adjustment;
		};

	//	static_assert(sizeof(FreeListHeader) == sizeof(AllocatedListHeader), "Both headers must have the same size");

	private:
		FreeListAllocator(const FreeListAllocator&) = delete;
		FreeListAllocator(FreeListAllocator&&) = delete;

		FreeListHeader* 	 _freeBlocks;
		AllocatedListHeader* _allocatedBlocks;

		void removeFromFreeList(FreeListHeader* oldFreeBlock, FreeListHeader* newFreeBlock);
		void removeFromAllocatedList(AllocatedListHeader* allocatedBlock);

		void* findPreviousInFreeList(void *ptr);
		void* findRawPreviousInFreeList(void *ptr);
		void* findNextInAllocatedList(void *ptr);
		void* findInFreeList(void* ptr);
		void* findInAllocatedList(void* ptr);

	public:
		FreeListAllocator(void* memory, std::size_t size);

		void* 		allocate(std::size_t size, std::size_t alignment, std::size_t offset) override;
		void*		resize(void* ptr, std::size_t newSize) override;
		void 		deallocate(void* ptr) override;
		std::size_t getAllocationSize(const void* ptr) const override;
		bool 		hasAddress(const void* ptr) const override;
	};
}

#endif //GAME_FREELISTALLOCATOR_H
