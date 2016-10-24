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
			void* 	 next;
			uint32_t size;
			uint8_t  adjustment;
		};

		FreeListAllocator(const FreeListAllocator&) = delete;
		FreeListAllocator(FreeListAllocator&&) = delete;

		FreeListHeader* _freeBlocks;

		void 			doRemoveFromList(FreeListHeader* list, FreeListHeader* ptr);
		FreeListHeader* getListEnd(FreeListHeader* list);

		void* findPreviousInFreeList(void *ptr);
		void* findRawPreviousInFreeList(void *ptr);
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
