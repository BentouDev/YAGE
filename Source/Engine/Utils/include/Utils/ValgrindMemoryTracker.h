//
// Created by bentoo on 10/21/16.
//

#ifndef GAME_VALGRINDMEMORYTRACKER_H
#define GAME_VALGRINDMEMORYTRACKER_H

#include "MemoryTracker.h"
#include "ValgrindMemoryBoundChecker.h"
#include <cstdint>
#include <valgrind/valgrind.h>
#include <valgrind/memcheck.h>

namespace Memory
{
	class ValgrindMemoryTracker : public IMemoryTracker
	{
	public:
		explicit ValgrindMemoryTracker(Memory::IMemoryBlock& memory)
			: IMemoryTracker(memory)
		{
			VALGRIND_CREATE_MEMPOOL(getSuperblockPtr(), ValgrindMemoryBoundChecker::BOUND_OFFSET, false);
		//	VALGRIND_MAKE_MEM_NOACCESS(getSuperblockPtr(), _block.getCapacity());
		}

		~ValgrindMemoryTracker()
		{
			VALGRIND_DESTROY_MEMPOOL(getSuperblockPtr());
		}

		inline void OnAllocation(void* ptr, std::size_t size, std::size_t, std::size_t frontOffset, const Utils::DebugSourceInfo&) override
		{
			const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(ptr) + frontOffset;
		//	VALGRIND_MALLOCLIKE_BLOCK(reinterpret_cast<void*>(address), size, frontOffset, false);
			VALGRIND_MEMPOOL_ALLOC(getSuperblockPtr(), reinterpret_cast<void*>(address), size); // + frontOffset
		};

		inline void OnDeallocation(void* ptr, std::size_t frontOffset) override
		{
			const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(ptr) + frontOffset;
		//	VALGRIND_FREELIKE_BLOCK(reinterpret_cast<void*>(address), frontOffset);
			VALGRIND_MEMPOOL_FREE(getSuperblockPtr(), reinterpret_cast<void*>(address)); // + frontOffset
		};
	};
}

#endif //GAME_VALGRINDMEMORYTRACKER_H
