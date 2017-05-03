//
// Created by bentoo on 14.11.16.
//

#ifndef GAME_MEMORYMODULE_H
#define GAME_MEMORYMODULE_H

#include <cstddef>
#include <cstdint>
#include <Utils/MemoryBlock.h>
#include <Utils/FreeListAllocator.h>
#include <Utils/SimpleMemoryBoundChecker.h>
#include <Utils/SimpleMemoryTracker.h>
#include <Utils/List.h>
#include <Utils/ValgrindMemoryBoundChecker.h>
#include <Utils/ValgrindMemoryTracker.h>

namespace Core
{
	class MemoryModule final
	{
	public:
		using master_block_t = Memory::MemoryBlock<Memory::FreeListAllocator,
												   Memory::SimpleMemoryBoundChecker,
												   Memory::SimpleMemoryTracker>;

		using user_block_t = Memory::MemoryBlock<Memory::FreeListAllocator,
												 Memory::SimpleMemoryBoundChecker,
												 Memory::SimpleMemoryTracker>;

	protected:
		struct MemoryBlockNode
		{
			inline explicit MemoryBlockNode(Memory::IMemoryBlock* ptr, MemoryBlockNode* next, void* memory)
				: Ptr(ptr), Next(next), Memory(memory)
			{ }

			Memory::IMemoryBlock*	Ptr;
			MemoryBlockNode*		Next;
			void*					Memory;
		};

		MemoryBlockNode*			_userBlocks;

		void*						_masterMemoryPtr;
		std::size_t					_masterMemorySize;
		Memory::FreeListAllocator*	_masterAllocator;
		Memory::IMemoryBlock*		_masterMemoryBlock;

		// temp memory
		// frame memory
		// engine memory (list of managers, etc.)

		void registerMemoryBlock(Memory::IMemoryBlock* block, void* memory);
		bool checkForLeak(Memory::IMemoryBlock* block);

	public:
		explicit MemoryModule(std::size_t allocationSize);
		~MemoryModule() noexcept;

		MemoryModule(MemoryModule&&) = delete;
		MemoryModule(const MemoryModule&) = delete;
		MemoryModule& operator=(MemoryModule&&) = delete;
		MemoryModule& operator=(const MemoryModule&) = delete;

		Memory::IMemoryBlock& masterBlock() const { return *_masterMemoryBlock; }
		Memory::IMemoryBlock& requestMemoryBlock(std::size_t blockSize, const char* name);
		bool freeMemoryBlock(Memory::IMemoryBlock* block);
		bool releaseAll();
	};
}

#endif //GAME_MEMORYMODULE_H
