//
// Created by bentoo on 10/21/16.
//

#ifndef GAME_VALGRINDMEMORYBOUNDCHECKER_H
#define GAME_VALGRINDMEMORYBOUNDCHECKER_H

#include "MemoryBoundChecker.h"
#include <valgrind/valgrind.h>

namespace Memory
{
	class ValgrindMemoryBoundChecker : public IMemoryBoundChecker
	{

	public:
		const static std::size_t BOUND_OFFSET = 4;

		inline std::size_t getSizeFront() const override
		{ return BOUND_OFFSET; };

		inline std::size_t getSizeBack() const override
		{ return BOUND_OFFSET; };

		inline void GuardFront(void*) override {}
		inline void GuardBack(void*) override {}

		inline void CheckFront(const void*) override {}
		inline void CheckBack(const void*) override {}
	};
}

#endif //GAME_VALGRINDMEMORYBOUNDCHECKER_H
