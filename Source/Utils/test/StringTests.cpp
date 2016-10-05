//
// Created by bentoo on 10/2/16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <malloc.h>
#include "Utils/MemoryBlock.h"
#include "Utils/FreeListAllocator.h"
#include "Utils/String.h"

namespace StringTests
{
	class StringTest : public ::testing::Test
	{
	public:
		void SetUp()
		{

		}

		void TearDown()
		{

		}
	};

	TEST_F(StringTest, CanCreateString)
	{
		const std::size_t 	memorySize 	= 1024;
		void* 				memory 		= malloc(memorySize);

		Memory::FreeListAllocator* allocator = new Memory::FreeListAllocator(memory, memorySize);
		Memory::MemoryBlock<Memory::FreeListAllocator> block(*allocator);

		Utils::String* s = new Utils::String(block);

		EXPECT_NE(nullptr, s);

		free(memory);
	}
}
