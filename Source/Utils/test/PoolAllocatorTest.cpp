//
// Created by bentoo on 9/29/16.
//

#include <catch.hpp>
#include <malloc.h>
#include "Utils/PoolAllocator.h"
#include "Utils/ScopeGuard.h"

namespace PoolAllocatorTest
{
	struct FooMock
	{
		int i;
		int j;
		char c;
	};

    TEST_CASE("PoolAllocatorTest")
    {
        const uint32_t memorySize = 1024;
        const uint32_t allocSize = sizeof(FooMock);
        const uint32_t alignSize = alignof(FooMock);
        const uint32_t offsetSize = 4;

        void* poolPtr;
        void* memory;

        poolPtr = malloc(sizeof(Memory::PoolAllocator));
        memory  = malloc(memorySize);

        YAGE_DISPOSE
        {
            free(poolPtr);
            free(memory);
        };

        SECTION("CanCreateAllocator")
        {
            auto allocator = Memory::PoolAllocator::create<FooMock>(poolPtr, memory, memorySize, 0);

            REQUIRE(nullptr != allocator);
        }

        SECTION("CanAllocateEnoughMemory")
        {
            auto allocator = Memory::PoolAllocator::create<FooMock>(poolPtr, memory, memorySize, 0);

            auto first = allocator->allocate(allocSize, alignSize, 0);
            auto second = allocator->allocate(allocSize, alignSize, 0);

            std::uintptr_t firstAddress = reinterpret_cast<std::uintptr_t>(first);
            std::uintptr_t secondAddress = reinterpret_cast<std::uintptr_t>(second);

            REQUIRE(secondAddress > firstAddress);
            REQUIRE(firstAddress + allocSize <= secondAddress);
        }

        SECTION("CanAllocateMemoryWithOffset")
        {
            auto allocator = Memory::PoolAllocator::create<FooMock>(poolPtr, memory, memorySize, offsetSize);

            auto first = allocator->allocate(allocSize, alignSize, offsetSize);
            auto second = allocator->allocate(allocSize, alignSize, offsetSize);

            REQUIRE(second > first);
            REQUIRE(reinterpret_cast<std::uintptr_t>(first) + allocSize + offsetSize
                     <= reinterpret_cast<std::uintptr_t>(second));
        }

        SECTION("CanReallocateMemory")
        {
            auto allocator = Memory::PoolAllocator::create<FooMock>(poolPtr, memory, memorySize, 0);

            const uint32_t emptySize = allocator->getUsedSize();

            auto first = allocator->allocate(allocSize, alignSize, 0);
            auto second = allocator->allocate(allocSize, alignSize, 0);

            allocator->deallocate(second);
            allocator->deallocate(first);

            REQUIRE(emptySize == allocator->getUsedSize());

            auto third = allocator->allocate(allocSize, alignSize, 0);
            auto fourth = allocator->allocate(allocSize, alignSize, 0);

            REQUIRE(first == third);
            REQUIRE(second == fourth);
        }

        SECTION("CanReallocateMemoryWithOffset")
        {
            auto allocator = Memory::PoolAllocator::create<FooMock>(poolPtr, memory, memorySize, offsetSize);

            const uint32_t emptySize = allocator->getUsedSize();

            auto first = allocator->allocate(allocSize, alignSize, offsetSize);
            auto second = allocator->allocate(allocSize, alignSize, offsetSize);

            allocator->deallocate(second);
            allocator->deallocate(first);

            REQUIRE(emptySize == allocator->getUsedSize());

            auto third = allocator->allocate(allocSize, alignSize, offsetSize);
            auto fourth = allocator->allocate(allocSize, alignSize, offsetSize);

            REQUIRE(first == third);
            REQUIRE(second == fourth);
        }
    }
}