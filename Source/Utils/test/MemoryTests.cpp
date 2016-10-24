//
// Created by mrjaqbq on 05.03.16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <malloc.h>
#include "Utils/StackAllocator.h"
#include "Utils/SafeDelete.h"
#include "Utils/MemoryBlock.h"

namespace MemoryTests
{
	typedef Memory::MemoryBlock<Memory::StackAllocator> MockMemory;

	class FooMock
	{
	public:
		MOCK_METHOD0(Die,  void());

		~FooMock()
		{
			Die();
		}
	};

	class MemoryTest : public ::testing::Test
	{
	public:

		void SetUp()
		{
			// code here will execute just before the test ensues
		}

		void TearDown()
		{
			// code here will be called just after the test completes
			// ok to through exceptions from here if need be
		}
	};

	TEST_F(MemoryTest, CanSafeDelete)
	{
		auto foo = new FooMock();

		EXPECT_CALL(*foo, Die());

		Memory::SafeDelete(foo);

		EXPECT_EQ(nullptr, foo);
	}

	TEST_F(MemoryTest, CanSafeDeleteTwice)
	{
		auto foo = new FooMock();

		EXPECT_CALL(*foo, Die()).Times(1);

		Memory::SafeDelete(foo);
		Memory::SafeDelete(foo);

		EXPECT_EQ(nullptr, foo);
	}

	TEST_F(MemoryTest, CanSafeDeleteArrayOfObjects)
	{
		int count = 6;
		auto foos = new FooMock[count];

		//for(auto i = 0; i < size; i++)
		//	auto obj = new (&foos[i]) FooMock();

		for(auto i = 0; i < count; i++)
			EXPECT_CALL(foos[i], Die()).Times(1);

		Memory::SafeDeleteArray(foos);

		EXPECT_EQ(nullptr, foos);
	}

	TEST_F(MemoryTest, CanSafeDeleteArrayOfObjectsTwice)
	{
		int count = 6;
		auto foos = new FooMock[count];

		//for(auto i = 0; i < size; i++)
		//	auto = new (foos[i]) FooMock();

		for(auto i = 0; i < count; i++)
			EXPECT_CALL(foos[i], Die()).Times(1);

		Memory::SafeDeleteArray(foos);
		Memory::SafeDeleteArray(foos);

		EXPECT_EQ(nullptr, foos);
	}

	TEST_F(MemoryTest, CanSafeFree)
	{
		auto memory = (FooMock*)malloc(sizeof(FooMock));
		auto foo = new(memory) FooMock();

		EXPECT_CALL(*foo, Die());

		Memory::SafeFree(foo);

		EXPECT_EQ(nullptr, foo);
	}

	TEST_F(MemoryTest, CanSafeFreeTwice)
	{
		auto memory = (FooMock*)malloc(sizeof(FooMock));
		auto foo = new(memory) FooMock();

		EXPECT_CALL(*foo, Die());

		Memory::SafeFree(foo);
		Memory::SafeFree(foo);

		EXPECT_EQ(nullptr, foo);
	}

	TEST_F(MemoryTest, CanSafeFreeArray)
	{
		int count = 4;
		FooMock* memory = (FooMock*)malloc(sizeof(FooMock) * count);

		for(auto i = 0; i < count; i++)
			new (&memory[i]) FooMock();

		for(auto i = 0; i < count; i++)
			EXPECT_CALL(memory[i], Die()).Times(1);

		Memory::SafeFreeArray(memory, count);

		EXPECT_EQ(nullptr, memory);
	}

	TEST_F(MemoryTest, CanSafeFreeArrayTwice)
	{
		int count = 4;
		FooMock* memory = (FooMock*)malloc(sizeof(FooMock) * count);

		for(auto i = 0; i < count; i++)
			new (&memory[i]) FooMock();

		for(auto i = 0; i < count; i++)
			EXPECT_CALL(memory[i], Die()).Times(1);

		Memory::SafeFreeArray(memory, count);
		Memory::SafeFreeArray(memory, count);

		EXPECT_EQ(nullptr, memory);
	}

	TEST_F(MemoryTest, CanAllocateMemoryFromBlock)
	{
		const std::size_t memorySize = 1024;
		void*			  memoryPtr  = malloc(memorySize);

		MockMemory block(*new Memory::StackAllocator(memoryPtr, memorySize));
		FooMock* foo = YAGE_CREATE_NEW(block, FooMock)();

		EXPECT_NE(nullptr, foo);

		free(memoryPtr);
	}

	TEST_F(MemoryTest, CanAllocateAndFreeMemoryFromBlock)
	{
		const std::size_t memorySize = 1024;
		void*			  memoryPtr  = malloc(memorySize);
		FooMock* 		  foo 		 = nullptr;

		MockMemory block(*new Memory::StackAllocator(memoryPtr, memorySize));
		foo = YAGE_CREATE_NEW(block, FooMock)();

		EXPECT_CALL(*foo, Die());

		Memory::Delete(block, foo);

		EXPECT_EQ(nullptr, foo);

		free(memoryPtr);
	}
}