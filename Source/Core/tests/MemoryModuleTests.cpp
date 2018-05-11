//
// Created by bentoo on 18.11.16.
//

#include <catch.hpp>

#include <Core/MemoryModule.h>
#include <Utils/MemorySizes.h>

namespace MemoryModuleTests
{
    TEST_CASE("MemoryModuleTest")
    {
        const std::size_t MemorySize = Memory::MB(1);

        SECTION("CanCreateModule")
        {
            auto* module = new Core::MemoryModule(MemorySize);

            REQUIRE(module != nullptr);
            REQUIRE(!module->releaseAll());

            delete module;
        }

        SECTION("CanCreateAndFreeMemoryBlock")
        {
            auto* module = new Core::MemoryModule(MemorySize);

            auto& block = module->requestMemoryBlock(Memory::KB(1), "CanCreateAndFreeMemoryBlock");

            char* blockPtr = reinterpret_cast<char*>(&block);
            char* allocPtr = reinterpret_cast<char*>(&block.getAllocator());

            REQUIRE(blockPtr > allocPtr);
            REQUIRE(allocPtr + sizeof(decltype(block.getAllocator())) < blockPtr);

            module->freeMemoryBlock(&block);

            REQUIRE(!module->releaseAll());

            delete module;
        }

        SECTION("CanCreateMemoryBlockOfSize")
        {
            auto* module = new Core::MemoryModule(MemorySize);
            auto& block = module->requestMemoryBlock(Memory::KB(1), "CanCreateMemoryBlockOfSize");

            REQUIRE(block.getFreeSize() == Memory::KB(1));

            delete module;
        }

        SECTION("DoesReportWhenLeaked")
        {
            auto* module = new Core::MemoryModule(MemorySize);
            auto& block = module->requestMemoryBlock(Memory::KB(1), "Leaked Block");
            auto* leakedMem = block.allocate(12, 2, DEBUG_SOURCE_INFO);

            REQUIRE(module->releaseAll());

            delete module;
        }
    }
}
