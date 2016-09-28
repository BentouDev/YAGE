//
// Created by bentoo on 9/28/16.
//

#ifndef GAME_POOLALLOCATOR_H
#define GAME_POOLALLOCATOR_H

#include "Allocator.h"

namespace Memory
{
	class PoolAllocator : public Allocator
	{
	public:
		PoolAllocator(void *memory, size_t size);

		void* 		allocate(std::size_t size, std::size_t alignment, std::size_t offset) override;
		void 		deallocate(void* ptr) override;
		std::size_t getAllocationSize(const void* ptr) const override;
		bool 		hasAddress(const void* ptr) const override;
	};
}

#endif //GAME_POOLALLOCATOR_H
