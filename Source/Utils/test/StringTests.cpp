//
// Created by bentoo on 10/2/16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <malloc.h>
#include "Utils/MemoryBlock.h"
#include "Utils/FreeListAllocator.h"
#include "Utils/String.h"

#if YAGE_VALGRIND
#include "Utils/ValgrindMemoryBoundChecker.h"
#include "Utils/ValgrindMemoryTracker.h"
#else
#include "Utils/SimpleMemoryBoundChecker.h"
#include "Utils/SimpleMemoryTracker.h"
#endif

namespace StringTests
{
#if YAGE_VALGRIND
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

	class StringTest : public ::testing::Test
	{
	protected:
		const std::size_t 	memorySize = 1024;
		void* 				memory;
		Memory::FreeListAllocator* allocator;

	public:
		void SetUp()
		{
			memory = malloc(memorySize);
			allocator = new Memory::FreeListAllocator(memory, memorySize);
		}

		void TearDown()
		{
			delete allocator;
			free(memory);
		}
	};

	TEST_F(StringTest, CanCreateString)
	{
		MockMemory block(*allocator, "CanCreateString");

		Utils::String* s = new Utils::String(block);

		EXPECT_NE(nullptr, s);

		delete s;
	}

	TEST_F(StringTest, CanInitializeString)
	{
		MockMemory block(*allocator, "CanInitializeString");

		Utils::String* s = new Utils::String(block, "Some sample string");

		EXPECT_NE(nullptr, s);

		delete s;
	}

	TEST_F(StringTest, CanAppendString)
	{
		MockMemory block(*allocator, "CanAppendString");

		Utils::String* s = new Utils::String(block);
		s->append("Some sample string");

		EXPECT_NE(nullptr, s);

		delete s;
	}

	TEST_F(StringTest, CanCreateMultipleStrings)
	{
		MockMemory block(*allocator, "CanCreateMultipleStrings");

		Utils::String* first = new Utils::String(block, "Some sample string");
		Utils::String* second = new Utils::String(block, "Other");
		first->append(" and stuff.");

		EXPECT_NE(nullptr, first);
		EXPECT_NE(nullptr, second);

		delete first;
		delete second;
	}

	TEST_F(StringTest, CanTokenizeString)
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
		EXPECT_EQ(block.getFreeSize(), free);
		{

			Utils::String str(block, "Foo\nBar\n\rBaz\nQ");
			Utils::List<Utils::Slice<char> >tokens(block);
			Utils::String::Tokenize(str, tokens, "\n\r");
			EXPECT_EQ(4, tokens.size());

			for (int i = 0; i < tokens.size() && i < 4; i++)
			{
				auto& slice = tokens[i];
				Utils::String expected(block, slice);
				EXPECT_STREQ(results[i], expected.c_str());
			}
		}
	}
}
