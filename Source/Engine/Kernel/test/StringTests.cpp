//
// Created by bentoo on 10/2/16.
//

#include <catch.hpp>
#include <malloc.h>
#include "Utils/MemoryBlock.h"
#include "Utils/FreeListAllocator.h"
#include "Utils/String.h"
#include "Utils/ScopeGuard.h"

#if YAGE_VALGRIND
#include "Utils/ValgrindMemoryBoundChecker.h"
#include "Utils/ValgrindMemoryTracker.h"
#else
#include "Utils/SimpleMemoryBoundChecker.h"
#include "Utils/SimpleMemoryTracker.h"
#endif

namespace StringTests
{
/*#if YAGE_VALGRIND
typedef Memory::MemoryBlock
			<Memory::FreeListAllocator,
			 Memory::ValgrindMemoryBoundChecker,
			 Memory::ValgrindMemoryTracker> MockMemory;
#else
typedef Memory::MemoryBlock <
		Memory::FreeListAllocator,
		Memory::SimpleMemoryBoundChecker,
		Memory::SimpleMemoryTracker> MockMemory;
#endif

    TEST_CASE ("StringTest")
	{
		const std::size_t 	memorySize = 1024;
		void* 				memory;
		Memory::FreeListAllocator* allocator;

		memory = malloc(memorySize);
		allocator = new Memory::FreeListAllocator(memory, memorySize);

		YAGE_DISPOSE
		{
			delete allocator;
			free(memory);
        };

        SECTION("CanCreateString")
        {
            MockMemory block(*allocator, "CanCreateString");

            Utils::String* s = new Utils::String(block);

            REQUIRE(nullptr != s);

            delete s;
        }

        SECTION("CanInitializeString")
        {
            MockMemory block(*allocator, "CanInitializeString");

            Utils::String* s = new Utils::String(block, "Some sample string");

            REQUIRE(nullptr != s);

            delete s;
        }

        SECTION("CanAppendString")
        {
            MockMemory block(*allocator, "CanAppendString");

            Utils::String* s = new Utils::String(block);
            s->append("Some sample string");

            REQUIRE(nullptr != s);

            delete s;
        }

        SECTION("CanCreateMultipleStrings")
        {
            MockMemory block(*allocator, "CanCreateMultipleStrings");

            Utils::String* first = new Utils::String(block, "Some sample string");
            Utils::String* second = new Utils::String(block, "Other");
            first->append(" and stuff.");

            REQUIRE(nullptr != first);
            REQUIRE(nullptr != second);

            delete first;
            delete second;
        }

        SECTION("CanTokenizeString")
        {
            const char* results[] = {
                "Foo", "Bar", "Baz", "Q"
            };

            MockMemory block(*allocator, "CanTokenizeString");
            auto free = block.getFreeSize();
            {
                Utils::String str(block, "Foo\nBar\n\rBaz\nQ");
                Utils::List<Utils::Slice<char> >tokens(block);
                Utils::String::Tokenize(str, tokens, "\n\r");
            }
            REQUIRE(block.getFreeSize() == free);
            {

                Utils::String str(block, "Foo\nBar\n\rBaz\nQ");
                Utils::List<Utils::Slice<char> >tokens(block);
                Utils::String::Tokenize(str, tokens, "\n\r");
                REQUIRE(4 == tokens.size());

                for (int i = 0; i < tokens.size() && i < 4; i++)
                {
                    auto& slice = tokens[i];
                    Utils::String expected(block, slice);
                    REQUIRE(strcmp(results[i], expected.c_str()) == 0);
                }
            }
        }
    }*/
}
