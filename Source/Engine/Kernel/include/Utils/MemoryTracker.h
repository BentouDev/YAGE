//
// Created by bentoo on 10/21/16.
//

#ifndef GAME_MEMORYTRACKER_H
#define GAME_MEMORYTRACKER_H

#include <cstdlib>
#include "DebugSourceInfo.h"

namespace Memory
{
	class IMemoryBlock;

	class IMemoryTracker
	{
	protected:
		Memory::IMemoryBlock& _block;
		explicit IMemoryTracker(Memory::IMemoryBlock& memory)
			: _block(memory)
		{ }

		virtual ~IMemoryTracker(){}

		void* getSuperblockPtr() const;
		const char* getName() const;

	public:
		virtual inline void OnAllocation(void*, std::size_t, std::size_t, std::size_t, const Utils::DebugSourceInfo&) = 0;
		virtual inline void OnDeallocation(void*, std::size_t) = 0;
	};

	class NoMemoryTracker : public IMemoryTracker
	{
	public:
		explicit NoMemoryTracker(Memory::IMemoryBlock& memory)
			: IMemoryTracker(memory)
		{ }

		inline void OnAllocation(void*, std::size_t, std::size_t, std::size_t, const Utils::DebugSourceInfo&) override {};
		inline void OnDeallocation(void*, std::size_t) override {};
	};
}

#endif //GAME_MEMORYTRACKER_H
