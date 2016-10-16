//
// Created by bentoo on 9/29/16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <malloc.h>
#include "Utils/PoolAllocator.h"

namespace PoolAllocatorTest
{
	struct FooMock
	{
		int i;
		int j;
		char c;
	};

	class PoolAllocatorTest : public ::testing::Test
	{
	public:
		const uint32_t memorySize = 1024;
		const uint32_t allocSize = sizeof(FooMock);
		const uint32_t alignSize = alignof(FooMock);
		const uint32_t offsetSize = 4;

		void* poolPtr;
		void* memory;

		void SetUp()
		{
			poolPtr = malloc(sizeof(Memory::PoolAllocator));
			memory = malloc(memorySize);
		}

		void TearDown()
		{
			free(poolPtr);
			free(memory);
		}
	};

	TEST_F(PoolAllocatorTest, CanCreateAllocator)
	{
		auto allocator 	= Memory::PoolAllocator::create<FooMock>(poolPtr, memory, memorySize, 0);

		EXPECT_NE(nullptr, allocator);
	}

	TEST_F(PoolAllocatorTest, CanAllocateEnoughMemory)
	{
		auto allocator 	= Memory::PoolAllocator::create<FooMock>(poolPtr, memory, memorySize, 0);

		auto first  = allocator->allocate(allocSize, alignSize, 0);
		auto second = allocator->allocate(allocSize, alignSize, 0);

		std::uintptr_t firstAddress  = reinterpret_cast<std::uintptr_t >(first);
		std::uintptr_t secondAddress = reinterpret_cast<std::uintptr_t >(second);

		EXPECT_TRUE(secondAddress > firstAddress);
		EXPECT_FALSE(firstAddress + allocSize > secondAddress);
	}

	TEST_F(PoolAllocatorTest, CanAllocateMemoryWithOffset)
	{
		auto allocator 	= Memory::PoolAllocator::create<FooMock>(poolPtr, memory, memorySize, offsetSize);

		auto first  = allocator->allocate(allocSize, alignSize, offsetSize);
		auto second = allocator->allocate(allocSize, alignSize, offsetSize);

		EXPECT_TRUE(second > first);
		EXPECT_FALSE(reinterpret_cast<std::uintptr_t>(first) + allocSize + offsetSize
					 > reinterpret_cast<std::uintptr_t >(second));
	}

	TEST_F(PoolAllocatorTest, CanReallocateMemory)
	{
		auto allocator 	= Memory::PoolAllocator::create<FooMock>(poolPtr, memory, memorySize, 0);

		const uint32_t emptySize = allocator->getUsedSize();

		auto first  = allocator->allocate(allocSize, alignSize, 0);
		auto second = allocator->allocate(allocSize, alignSize, 0);

		allocator->deallocate(second);
		allocator->deallocate(first);

		EXPECT_EQ(emptySize, allocator->getUsedSize());

		auto third  = allocator->allocate(allocSize, alignSize, 0);
		auto fourth = allocator->allocate(allocSize, alignSize, 0);

		EXPECT_EQ(first, third);
		EXPECT_EQ(second, fourth);
	}

	TEST_F(PoolAllocatorTest, CanReallocateMemoryWithOffset)
	{
		auto allocator 	= Memory::PoolAllocator::create<FooMock>(poolPtr, memory, memorySize, offsetSize);

		const uint32_t emptySize = allocator->getUsedSize();

		auto first  = allocator->allocate(allocSize, alignSize, offsetSize);
		auto second = allocator->allocate(allocSize, alignSize, offsetSize);

		allocator->deallocate(second);
		allocator->deallocate(first);

		EXPECT_EQ(emptySize, allocator->getUsedSize());

		auto third  = allocator->allocate(allocSize, alignSize, offsetSize);
		auto fourth = allocator->allocate(allocSize, alignSize, offsetSize);

		EXPECT_EQ(first, third);
		EXPECT_EQ(second, fourth);
	}
}