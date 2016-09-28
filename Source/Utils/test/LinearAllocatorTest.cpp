//
// Created by bentoo on 9/28/16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <malloc.h>
#include "../include/LinearAllocator.h"

namespace LinearAllocatorTests
{
	class LinearAllocatorTest : public ::testing::Test
	{
	public:
		void SetUp()
		{

		}

		void TearDown()
		{

		}
	};

	TEST_F(LinearAllocatorTest, CanCreateAllocator)
	{
		const uint32_t memorySize = 1024;

		void* memory = malloc(memorySize);
		auto allocator = new Memory::LinearAllocator(memory, memorySize);

		EXPECT_NE(allocator, nullptr);
	}

	TEST_F(LinearAllocatorTest, CanAllocateEnoughMemory)
	{
		const uint32_t memorySize = 32;
		const uint32_t allocSize = 10;
		const uint32_t alignSize = 2;

		void* memory	= malloc(memorySize);
		auto allocator 	= new Memory::LinearAllocator(memory, memorySize);

		auto first  = allocator->allocate(allocSize, alignSize, 0);
		auto second = allocator->allocate(allocSize, alignSize, 0);

		std::uintptr_t firstAddress  = reinterpret_cast<std::uintptr_t >(first);
		std::uintptr_t secondAddress = reinterpret_cast<std::uintptr_t >(second);

		EXPECT_TRUE(secondAddress > firstAddress);
		EXPECT_FALSE(firstAddress + allocSize > secondAddress);
	}

	TEST_F(LinearAllocatorTest, CanAllocateMemoryWithOffset)
	{
		const uint32_t memorySize = 64;
		const uint32_t allocSize = 10;
		const uint32_t alignSize = 2;
		const uint32_t offsetSize = 8;

		void* memory	= malloc(memorySize);
		auto allocator 	= new Memory::LinearAllocator(memory, memorySize);

		auto first  = allocator->allocate(allocSize, alignSize, offsetSize);
		auto second = allocator->allocate(allocSize, alignSize, offsetSize);

		EXPECT_TRUE(second > first);
		EXPECT_FALSE(reinterpret_cast<std::uintptr_t>(first) + allocSize + offsetSize
					 > reinterpret_cast<std::uintptr_t >(second));
	}

	TEST_F(LinearAllocatorTest, CanReallocateMemory)
	{
		const uint32_t memorySize = 32;
		const uint32_t allocSize = 10;
		const uint32_t alignSize = 2;

		void* memory 	= malloc(memorySize);
		auto allocator 	= new Memory::LinearAllocator(memory, memorySize);

		auto first  = allocator->allocate(allocSize, alignSize, 0);
		auto second = allocator->allocate(allocSize, alignSize, 0);

		allocator->clear();

		auto third  = allocator->allocate(allocSize, alignSize, 0);
		auto fourth = allocator->allocate(allocSize, alignSize, 0);

		EXPECT_EQ(first, third);
		EXPECT_EQ(second, fourth);
	}
}
