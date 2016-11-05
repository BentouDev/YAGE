//
// Created by bentoo on 10/21/16.
//

#ifndef GAME_VALGRINDMEMORYTRACKER_H
#define GAME_VALGRINDMEMORYTRACKER_H

#include "MemoryTracker.h"
#include "ValgrindMemoryBoundChecker.h"
#include <valgrind/valgrind.h>
#include <cstdint>

namespace Memory
{
	class ValgrindMemoryTracker : public IMemoryTracker
	{
	public:
		explicit ValgrindMemoryTracker(Memory::IMemoryBlock& memory)
			: IMemoryTracker(memory)
		{
			VALGRIND_CREATE_MEMPOOL(&_block, ValgrindMemoryBoundChecker::BOUND_OFFSET, false);
		}

		inline void OnAllocation(void* ptr, std::size_t size, std::size_t alignment, std::size_t frontOffset, const Utils::DebugSourceInfo&) override
		{
			const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(ptr);
			VALGRIND_MEMPOOL_ALLOC(&_block, reinterpret_cast<void*>(address), size); // + frontOffset
		};

		inline void OnDeallocation(void* ptr, std::size_t frontOffset) override
		{
			const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(ptr);
			VALGRIND_MEMPOOL_FREE(&_block, reinterpret_cast<void*>(address)); // + frontOffset
		};
	};
}

#endif //GAME_VALGRINDMEMORYTRACKER_H
