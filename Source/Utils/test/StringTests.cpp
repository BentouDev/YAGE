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

	public:
		void SetUp()
		{
			memory 		= malloc(memorySize);
		}

		void TearDown()
		{
			free(memory);
		}
	};

	TEST_F(StringTest, CanCreateString)
	{
		Memory::FreeListAllocator* allocator = new Memory::FreeListAllocator(memory, memorySize);
		MockMemory block(*allocator);

		Utils::String* s = new Utils::String(block);

		EXPECT_NE(nullptr, s);

		delete s;
	}

	TEST_F(StringTest, CanInitializeString)
	{
		Memory::FreeListAllocator* allocator = new Memory::FreeListAllocator(memory, memorySize);
		MockMemory block(*allocator);

		Utils::String* s = new Utils::String(block, "Some sample string");

		EXPECT_NE(nullptr, s);

		delete s;
	}

	TEST_F(StringTest, CanAppendString)
	{
		Memory::FreeListAllocator* allocator = new Memory::FreeListAllocator(memory, memorySize);
		MockMemory block(*allocator);

		Utils::String* s = new Utils::String(block);
		s->append("Some sample string");

		EXPECT_NE(nullptr, s);

		delete s;
	}

	TEST_F(StringTest, CanCreateMultipleStrings)
	{
		Memory::FreeListAllocator* allocator = new Memory::FreeListAllocator(memory, memorySize);
		MockMemory block(*allocator);

		Utils::String* first = new Utils::String(block, "Some sample string");
		Utils::String* second = new Utils::String(block, "Other");
		first->append(" and stuff.");

		EXPECT_NE(nullptr, first);
		EXPECT_NE(nullptr, second);

		delete first;
		delete second;
	}
}
