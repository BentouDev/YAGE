//
// Created by bentoo on 17.11.16.
//

#include <malloc.h>
#include <Utils/SafeDelete.h>
#include <Utils/LinearAllocator.h>
#include <Platform/Logger.h>
#include "Core/MemoryModule.h"

namespace Core
{
    MemoryModule::MemoryModule(std::size_t allocationSize)
        : _userBlocks(nullptr), _masterMemoryPtr(nullptr),
          _masterMemorySize(allocationSize), _masterMemoryBlock(nullptr)
    {
        _masterMemoryPtr	= malloc(_masterMemorySize);
        _masterAllocator	= new Memory::FreeListAllocator(_masterMemoryPtr, _masterMemorySize);
        _masterMemoryBlock	= new master_block_t (*_masterAllocator, "MemoryModule Master Block");
    }

    MemoryModule::~MemoryModule() noexcept
    {
        releaseAll();
    }

    bool MemoryModule::releaseAll()
    {
        bool result = false;
        MemoryBlockNode* node = _userBlocks;
        MemoryBlockNode* next = nullptr;

        while (node != nullptr)
        {
            Core::Logger::trace("All : Free mem block {} at {}", node->Block->getName(), (void*)node);

            result |= freeMemoryBlock(node->Block);

            next = node->Next;

            Memory::SafeDelete(node);
            node = next;
        }

        Memory::SafeDelete(_masterMemoryBlock);
        Memory::SafeDelete(_masterAllocator);
        Memory::SafeFree(_masterMemoryPtr);

        return result;
    }

    bool MemoryModule::checkForLeak(Memory::IMemoryBlock *block)
    {
        if(block == nullptr)
            return false;

        Memory::IAllocator& allocator = block->getAllocator();
        if (allocator.getUsedSize() != 0)
        {
            std::fprintf(stderr, "Block '%s' leaked '%zu' bytes\n", block->getName(), allocator.getUsedSize());
            return true;
        }

        return false;
    }

    bool MemoryModule::freeMemoryBlock(Memory::IMemoryBlock* block)
    {
        if (block == nullptr)
            return false;

        MemoryBlockNode** ptr_to_node = &_userBlocks;
        while (*ptr_to_node != nullptr && (*ptr_to_node)->Block != block)
            ptr_to_node = &((*ptr_to_node)->Next);

        if (*ptr_to_node != nullptr && (*ptr_to_node)->Block != nullptr)
        {
            void* memory = (*ptr_to_node)->Memory;
            (*ptr_to_node) = (*ptr_to_node)->Next;

            Core::Logger::trace("Free mem block {} at {}", block->getName(), (void*)(*ptr_to_node));

            bool leaked = checkForLeak(block);

            Memory::IAllocator& allocator = block->getAllocator();
            block->~IMemoryBlock();
            allocator.~IAllocator();

            _masterMemoryBlock->deallocate(memory);

            return leaked;
        }

        return false;
    }

    void MemoryModule::registerMemoryBlock(Memory::IMemoryBlock *block, void* memoryStart)
    {
        MemoryBlockNode* node = new MemoryBlockNode(block, _userBlocks, memoryStart);
        _userBlocks = node;

        Core::Logger::trace("New mem block {} node at {}", block->getName(), (void*)node);
    }

    Memory::IMemoryBlock& MemoryModule::requestMemoryBlock(std::size_t blockSize, const char* name)
    {
        // -> allocator | block | memory
        const std::size_t totalSize = sizeof(user_block_t) + sizeof(Memory::FreeListAllocator) + blockSize;

        char* allocatorChunk = reinterpret_cast<char*>(_masterMemoryBlock->allocate(totalSize, alignof(Memory::FreeListAllocator), DEBUG_SOURCE_INFO));
        Memory::LinearAllocator linear(allocatorChunk, totalSize);

        void* allocChunk = linear.allocate(sizeof(Memory::FreeListAllocator), alignof(Memory::FreeListAllocator), 0);
        void* blockChunk = linear.allocate(sizeof(user_block_t), alignof(user_block_t), 0);
        void* userChunk	 = reinterpret_cast<char*>(blockChunk) + sizeof(user_block_t);

        auto* allocator = new (allocChunk) Memory::FreeListAllocator(userChunk, linear.getFreeSize());
        auto* block = new (blockChunk) user_block_t(*allocator, name);

    //	Memory::FreeListAllocator*	allocator	= new (allocatorChunk) Memory::FreeListAllocator(memoryChunk, blockSize);
    //	Memory::IMemoryBlock*		block		= new (blockChunk) MemoryModule::user_block_t(*allocator, name);

        registerMemoryBlock(block, allocatorChunk);

        return *block;
    }
}
