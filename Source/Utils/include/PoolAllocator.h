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

	};
}

#endif //GAME_POOLALLOCATOR_H
