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
		FreeListAllocator(void* memory, size_t size);

	};
}

#endif //GAME_FREELISTALLOCATOR_H
