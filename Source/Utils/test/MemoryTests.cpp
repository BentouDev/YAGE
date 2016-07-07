//
// Created by mrjaqbq on 05.03.16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdlib.h>
#include <malloc.h>
#include "../include/SafeDelete.h"

namespace MemoryTests
{
	class FooMock
	{
	public:
		MOCK_METHOD0(Die, void());

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

		EXPECT_EQ(foo, nullptr);
	}

	TEST_F(MemoryTest, CanSafeDeleteTwice)
	{
		auto foo = new FooMock();

		EXPECT_CALL(*foo, Die()).Times(1);

		Memory::SafeDelete(foo);
		Memory::SafeDelete(foo);

		EXPECT_EQ(foo, nullptr);
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

		EXPECT_EQ(foos, nullptr);
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

		EXPECT_EQ(foos, nullptr);
	}

	TEST_F(MemoryTest, CanSafeFree)
	{
		auto memory = (FooMock*)malloc(sizeof(FooMock));
		auto foo = new(memory) FooMock();

		EXPECT_CALL(*foo, Die());

		Memory::SafeFree(foo);

		EXPECT_EQ(foo, nullptr);
	}

	TEST_F(MemoryTest, CanSafeFreeTwice)
	{
		auto memory = (FooMock*)malloc(sizeof(FooMock));
		auto foo = new(memory) FooMock();

		EXPECT_CALL(*foo, Die());

		Memory::SafeFree(foo);
		Memory::SafeFree(foo);

		EXPECT_EQ(foo, nullptr);
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

		EXPECT_EQ(memory, nullptr);
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

		EXPECT_EQ(memory, nullptr);
	}
}