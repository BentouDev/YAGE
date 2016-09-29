//
// Created by bentoo on 9/29/16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <malloc.h>
#include "../include/FreeListAllocator.h"

namespace FreeListAllocatorTests
{
	class FreeListAllocatorTest : public ::testing::Test
	{
	public:
		void SetUp()
		{

		}

		void TearDown()
		{

		}
	};

	TEST_F(FreeListAllocatorTest, CanCreateAllocator)
	{
		const uint32_t memorySize = 1024;

		void* memory = malloc(memorySize);
		auto allocator = new Memory::FreeListAllocator(memory, memorySize);

		EXPECT_NE(nullptr, allocator);

		free(memory);
	}

	TEST_F(FreeListAllocatorTest, CanAllocateEnoughMemory)
	{
		const uint32_t memorySize = 128;
		const uint32_t allocSize = 10;
		const uint32_t alignSize = 2;

		void* memory	= malloc(memorySize);
		auto allocator 	= new Memory::FreeListAllocator(memory, memorySize);

		auto first  = allocator->allocate(allocSize, alignSize, 0);
		auto second = allocator->allocate(allocSize, alignSize, 0);

		std::uintptr_t firstAddress  = reinterpret_cast<std::uintptr_t >(first);
		std::uintptr_t secondAddress = reinterpret_cast<std::uintptr_t >(second);

		EXPECT_TRUE(secondAddress > firstAddress);
		EXPECT_FALSE(firstAddress + allocSize > secondAddress);

		free(memory);
	}

	TEST_F(FreeListAllocatorTest, CanAllocateMemoryWithOffset)
	{
		const uint32_t memorySize = 128;
		const uint32_t allocSize = 10;
		const uint32_t alignSize = 2;
		const uint32_t offsetSize = 4;

		void* memory	= malloc(memorySize);
		auto allocator 	= new Memory::FreeListAllocator(memory, memorySize);

		auto first  = allocator->allocate(allocSize, alignSize, offsetSize);
		auto second = allocator->allocate(allocSize, alignSize, offsetSize);

		EXPECT_TRUE(second > first);
		EXPECT_FALSE(reinterpret_cast<std::uintptr_t>(first) + allocSize + offsetSize
					 > reinterpret_cast<std::uintptr_t >(second));

		free(memory);
	}

	TEST_F(FreeListAllocatorTest, CanReallocateMemory)
	{
		const uint32_t memorySize = 128;
		const uint32_t allocSize = 10;
		const uint32_t alignSize = 2;

		void* memory 	= malloc(memorySize);
		auto allocator 	= new Memory::FreeListAllocator(memory, memorySize);

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

		free(memory);
	}

	TEST_F(FreeListAllocatorTest, CanReallocateMemoryWithOffset)
	{
		const uint32_t memorySize = 128;
		const uint32_t allocSize = 10;
		const uint32_t alignSize = 2;
		const uint32_t offsetSize = 4;

		void* memory 	= malloc(memorySize);
		auto allocator 	= new Memory::FreeListAllocator(memory, memorySize);

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

		free(memory);
	}

	TEST_F(FreeListAllocatorTest, CanReallocateInOtherOrder)
	{
		const uint32_t memorySize = 1024;
		const uint32_t allocSize = 10;
		const uint32_t alignSize = 2;
		const uint32_t offsetSize = 4;

		void* memory 	= malloc(memorySize);
		auto allocator 	= new Memory::FreeListAllocator(memory, memorySize);

		const uint32_t emptySize = allocator->getUsedSize();

		auto first	 = allocator->allocate(allocSize, alignSize, offsetSize);
		auto second	 = allocator->allocate(allocSize, alignSize, offsetSize);
		auto third	 = allocator->allocate(allocSize, alignSize, offsetSize);
		auto fourth	 = allocator->allocate(allocSize, alignSize, offsetSize);

		allocator->deallocate(second);
		allocator->deallocate(fourth);
		allocator->deallocate(third);
		allocator->deallocate(first);

		EXPECT_EQ(emptySize, allocator->getUsedSize());

		auto fifth	 = allocator->allocate(allocSize, alignSize, offsetSize);
		auto sixth	 = allocator->allocate(allocSize, alignSize, offsetSize);
		auto seventh = allocator->allocate(allocSize, alignSize, offsetSize);
		auto eighth	 = allocator->allocate(allocSize, alignSize, offsetSize);

		EXPECT_EQ(first, fifth);
		EXPECT_EQ(second, sixth);
		EXPECT_EQ(third, seventh);
		EXPECT_EQ(fourth, eighth);

		free(memory);
	}
}