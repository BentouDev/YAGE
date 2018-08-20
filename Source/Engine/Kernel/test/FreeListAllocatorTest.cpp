//
// Created by bentoo on 9/29/16.
//

#include <catch.hpp>
#include <malloc.h>
#include "Utils/FreeListAllocator.h"
#include "Utils/ScopeGuard.h"

namespace FreeListAllocatorTests
{
    TEST_CASE("FreeListAllocatorTest")
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
            auto allocator = new Memory::FreeListAllocator(memory, memorySize);

            REQUIRE(nullptr != allocator);
        }

        SECTION("CanAllocateEnoughMemory")
        {
            auto allocator = new Memory::FreeListAllocator(memory, memorySize);

            auto first = allocator->allocate(allocSize, alignSize, 0);
            auto second = allocator->allocate(allocSize, alignSize, 0);

            std::uintptr_t firstAddress = reinterpret_cast<std::uintptr_t>(first);
            std::uintptr_t secondAddress = reinterpret_cast<std::uintptr_t>(second);

            REQUIRE(allocSize == allocator->getAllocationSize(first));
            REQUIRE(allocSize == allocator->getAllocationSize(second));
            REQUIRE(secondAddress > firstAddress);
            REQUIRE(firstAddress + allocSize <= secondAddress);
        }

        SECTION("CanAllocateMemoryWithOffset")
        {
            auto allocator = new Memory::FreeListAllocator(memory, memorySize);

            auto first = allocator->allocate(allocSize, alignSize, offsetSize);
            auto second = allocator->allocate(allocSize, alignSize, offsetSize);

            REQUIRE(allocSize == allocator->getAllocationSize(first));
            REQUIRE(allocSize == allocator->getAllocationSize(second));
            REQUIRE(second > first);
            REQUIRE(reinterpret_cast<std::uintptr_t>(first) + allocSize + offsetSize
                     <= reinterpret_cast<std::uintptr_t>(second));
        }

        SECTION("CanReallocateMemory")
        {
            auto allocator = new Memory::FreeListAllocator(memory, memorySize);

            const uint32_t emptySize = allocator->getUsedSize();

            auto first = allocator->allocate(allocSize, alignSize, 0);
            auto second = allocator->allocate(allocSize, alignSize, 0);

            REQUIRE(allocSize == allocator->getAllocationSize(first));
            REQUIRE(allocSize == allocator->getAllocationSize(second));

            allocator->deallocate(second);
            allocator->deallocate(first);

            REQUIRE(emptySize == allocator->getUsedSize());

            auto third = allocator->allocate(allocSize, alignSize, 0);
            auto fourth = allocator->allocate(allocSize, alignSize, 0);

            REQUIRE(allocSize == allocator->getAllocationSize(third));
            REQUIRE(allocSize == allocator->getAllocationSize(fourth));
            REQUIRE(first == third);
            REQUIRE(second == fourth);
        }

        SECTION("CanReallocateMemoryWithOffset")
        {
            auto allocator = new Memory::FreeListAllocator(memory, memorySize);

            const uint32_t emptySize = allocator->getUsedSize();

            auto first = allocator->allocate(allocSize, alignSize, offsetSize);
            auto second = allocator->allocate(allocSize, alignSize, offsetSize);

            REQUIRE(allocSize == allocator->getAllocationSize(first));
            REQUIRE(allocSize == allocator->getAllocationSize(second));

            allocator->deallocate(second);
            allocator->deallocate(first);

            REQUIRE(emptySize == allocator->getUsedSize());

            auto third = allocator->allocate(allocSize, alignSize, offsetSize);
            auto fourth = allocator->allocate(allocSize, alignSize, offsetSize);

            REQUIRE(allocSize == allocator->getAllocationSize(third));
            REQUIRE(allocSize == allocator->getAllocationSize(fourth));
            REQUIRE(first == third);
            REQUIRE(second == fourth);
        }

        SECTION("CanReallocateInOtherOrder")
        {
            auto allocator = new Memory::FreeListAllocator(memory, memorySize);

            const uint32_t emptySize = allocator->getUsedSize();

            auto first = allocator->allocate(allocSize, alignSize, 0);
            auto second = allocator->allocate(allocSize, alignSize, 0);
            auto third = allocator->allocate(allocSize, alignSize, 0);
            auto fourth = allocator->allocate(allocSize, alignSize, 0);

            allocator->deallocate(second);
            allocator->deallocate(fourth);
            allocator->deallocate(third);
            allocator->deallocate(first);

            REQUIRE(emptySize == allocator->getUsedSize());

            auto fifth = allocator->allocate(allocSize, alignSize, 0);
            auto sixth = allocator->allocate(allocSize, alignSize, 0);
            auto seventh = allocator->allocate(allocSize, alignSize, 0);
            auto eighth = allocator->allocate(allocSize, alignSize, 0);

            REQUIRE(allocSize == allocator->getAllocationSize(fifth));
            REQUIRE(allocSize == allocator->getAllocationSize(sixth));
            REQUIRE(allocSize == allocator->getAllocationSize(seventh));
            REQUIRE(allocSize == allocator->getAllocationSize(eighth));

            REQUIRE(first == fifth);
            REQUIRE(second == sixth);
            REQUIRE(third == seventh);
            REQUIRE(fourth == eighth);
        }

        SECTION("CanReallocateInOtherOrderWithOffset")
        {
            auto allocator = new Memory::FreeListAllocator(memory, memorySize);

            const uint32_t emptySize = allocator->getUsedSize();

            auto first = allocator->allocate(allocSize, alignSize, offsetSize);
            auto second = allocator->allocate(allocSize, alignSize, offsetSize);
            auto third = allocator->allocate(allocSize, alignSize, offsetSize);
            auto fourth = allocator->allocate(allocSize, alignSize, offsetSize);

            allocator->deallocate(second);
            allocator->deallocate(fourth);
            allocator->deallocate(third);
            allocator->deallocate(first);

            REQUIRE(emptySize == allocator->getUsedSize());

            auto fifth = allocator->allocate(allocSize, alignSize, offsetSize);
            auto sixth = allocator->allocate(allocSize, alignSize, offsetSize);
            auto seventh = allocator->allocate(allocSize, alignSize, offsetSize);
            auto eighth = allocator->allocate(allocSize, alignSize, offsetSize);

            REQUIRE(allocSize == allocator->getAllocationSize(fifth));
            REQUIRE(allocSize == allocator->getAllocationSize(sixth));
            REQUIRE(allocSize == allocator->getAllocationSize(seventh));
            REQUIRE(allocSize == allocator->getAllocationSize(eighth));

            REQUIRE(first == fifth);
            REQUIRE(second == sixth);
            REQUIRE(third == seventh);
            REQUIRE(fourth == eighth);
        }
    }
}