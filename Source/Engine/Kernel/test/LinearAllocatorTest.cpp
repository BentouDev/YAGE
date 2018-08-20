//
// Created by bentoo on 9/28/16.
//

#include <catch.hpp>
#include <malloc.h>
#include "Utils/LinearAllocator.h"
#include "Utils/ScopeGuard.h"

namespace LinearAllocatorTests
{
    TEST_CASE("LinearAllocatorTest")
    {
        const uint32_t memorySize = 1024;
        const uint32_t allocSize = 10;
        const uint32_t alignSize = 2;
        const uint32_t offsetSize = 4;
        void* memory;

        memory = malloc(memorySize);

        YAGE_DISPOSE
        {
            free(memory);
        };

        SECTION("CanCreateAllocator")
        {
            auto allocator = new Memory::LinearAllocator(memory, memorySize);

            REQUIRE(nullptr != allocator);
        }

        SECTION("CanAllocateEnoughMemory")
        {
            auto allocator = new Memory::LinearAllocator(memory, memorySize);

            auto first = allocator->allocate(allocSize, alignSize, 0);
            auto second = allocator->allocate(allocSize, alignSize, 0);

            std::uintptr_t firstAddress = reinterpret_cast<std::uintptr_t>(first);
            std::uintptr_t secondAddress = reinterpret_cast<std::uintptr_t>(second);

            REQUIRE(secondAddress > firstAddress);
            REQUIRE(firstAddress + allocSize <= secondAddress);
        }

        SECTION("CanAllocateMemoryWithOffset")
        {
            auto allocator = new Memory::LinearAllocator(memory, memorySize);

            auto first = allocator->allocate(allocSize, alignSize, offsetSize);
            auto second = allocator->allocate(allocSize, alignSize, offsetSize);

            REQUIRE(second > first);
            REQUIRE(reinterpret_cast<std::uintptr_t>(first) + allocSize + offsetSize
                     <= reinterpret_cast<std::uintptr_t>(second));
        }

        SECTION("CanReallocateMemory")
        {
            auto allocator = new Memory::LinearAllocator(memory, memorySize);

            auto first = allocator->allocate(allocSize, alignSize, 0);
            auto second = allocator->allocate(allocSize, alignSize, 0);

            allocator->clear();

            auto third = allocator->allocate(allocSize, alignSize, 0);
            auto fourth = allocator->allocate(allocSize, alignSize, 0);

            REQUIRE(first == third);
            REQUIRE(second == fourth);
        }

        SECTION("CanReallocateMemoryWithOffset")
        {
            auto allocator = new Memory::LinearAllocator(memory, memorySize);

            auto first = allocator->allocate(allocSize, alignSize, offsetSize);
            auto second = allocator->allocate(allocSize, alignSize, offsetSize);

            allocator->clear();

            auto third = allocator->allocate(allocSize, alignSize, offsetSize);
            auto fourth = allocator->allocate(allocSize, alignSize, offsetSize);

            REQUIRE(first == third);
            REQUIRE(second == fourth);
        }
    }
}
