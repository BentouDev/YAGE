//
// Created by bentoo on 9/29/16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <malloc.h>
#include "../include/PoolAllocator.h"

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
		void SetUp()
		{

		}

		void TearDown()
		{

		}
	};

	TEST_F(PoolAllocatorTest, CanCreateAllocator)
	{
		const uint32_t memorySize = 1024;

		void* poolPtr 	= malloc(sizeof(Memory::PoolAllocator));
		void* memory 	= malloc(memorySize);
		auto allocator 	= Memory::PoolAllocator::create<FooMock>(poolPtr, memory, memorySize, 0);

		EXPECT_NE(nullptr, allocator);

		free(poolPtr);
		free(memory);
	}

	TEST_F(PoolAllocatorTest, CanAllocateEnoughMemory)
	{
		const uint32_t memorySize = 64;
		const uint32_t allocSize = sizeof(FooMock);
		const uint32_t alignSize = alignof(FooMock);

		void* poolPtr 	= malloc(sizeof(Memory::PoolAllocator));
		void* memory 	= malloc(memorySize);
		auto allocator 	= Memory::PoolAllocator::create<FooMock>(poolPtr, memory, memorySize, 0);

		auto first  = allocator->allocate(allocSize, alignSize, 0);
		auto second = allocator->allocate(allocSize, alignSize, 0);

		std::uintptr_t firstAddress  = reinterpret_cast<std::uintptr_t >(first);
		std::uintptr_t secondAddress = reinterpret_cast<std::uintptr_t >(second);

		EXPECT_TRUE(secondAddress > firstAddress);
		EXPECT_FALSE(firstAddress + allocSize > secondAddress);

		free(poolPtr);
		free(memory);
	}

	TEST_F(PoolAllocatorTest, CanAllocateMemoryWithOffset)
	{
		const uint32_t memorySize = 64;
		const uint32_t allocSize = sizeof(FooMock);
		const uint32_t alignSize = alignof(FooMock);
		const uint32_t offsetSize = 4;

		void* poolPtr 	= malloc(sizeof(Memory::PoolAllocator));
		void* memory 	= malloc(memorySize);
		auto allocator 	= Memory::PoolAllocator::create<FooMock>(poolPtr, memory, memorySize, offsetSize);

		auto first  = allocator->allocate(allocSize, alignSize, offsetSize);
		auto second = allocator->allocate(allocSize, alignSize, offsetSize);

		EXPECT_TRUE(second > first);
		EXPECT_FALSE(reinterpret_cast<std::uintptr_t>(first) + allocSize + offsetSize
					 > reinterpret_cast<std::uintptr_t >(second));

		free(poolPtr);
		free(memory);
	}

	TEST_F(PoolAllocatorTest, CanReallocateMemory)
	{
		const uint32_t memorySize = 64;
		const uint32_t allocSize = sizeof(FooMock);
		const uint32_t alignSize = alignof(FooMock);

		void* poolPtr 	= malloc(sizeof(Memory::PoolAllocator));
		void* memory 	= malloc(memorySize);
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

		free(poolPtr);
		free(memory);
	}

	TEST_F(PoolAllocatorTest, CanReallocateMemoryWithOffset)
	{
		const uint32_t memorySize = 64;
		const uint32_t allocSize = sizeof(FooMock);
		const uint32_t alignSize = alignof(FooMock);
		const uint32_t offsetSize = 4;

		void* poolPtr 	= malloc(sizeof(Memory::PoolAllocator));
		void* memory 	= malloc(memorySize);
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

		free(poolPtr);
		free(memory);
	}
}