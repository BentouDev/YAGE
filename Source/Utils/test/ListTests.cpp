//
// Created by bentoo on 10/1/16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <malloc.h>
#include "Utils/FreeListAllocator.h"
#include "Utils/List.h"

#ifdef YAGE_VALGRIND
#include <Utils/ValgrindMemoryBoundChecker.h>
#include <Utils/ValgrindMemoryTracker.h>
#endif

namespace ListTests
{
#ifdef YAGE_VALGRIND
	typedef Memory::MemoryBlock
			<Memory::FreeListAllocator,
			 Memory::ValgrindMemoryBoundChecker,
			 Memory::ValgrindMemoryTracker> MockMemory;
#else
	typedef Memory::MemoryBlock <Memory::FreeListAllocator> MockMemory;
#endif

	class FooMock
	{
		uint32_t magicNumber;

	public:
		MOCK_METHOD0(Die, void());
		MOCK_METHOD0(Foo, void());

		static const uint32_t fooConst;

		uint32_t Quack()
		{
			Foo();
			return magicNumber;
		}

		FooMock()
		{
			magicNumber = fooConst;
		}

		~FooMock()
		{
			Die();
		}
	};

	const uint32_t FooMock::fooConst = 0xF00BACEC;

	class ListTest : public ::testing::Test
	{
	public:
		const std::size_t listCapacity = 4;
		const std::size_t memorySize = 4096;
		void*			  memoryPtr;

		Memory::FreeListAllocator* allocator;

		MockMemory* block;

		MockMemory& getMemory() const
		{
			return *block;
		}

		void SetUp()
		{
			memoryPtr = malloc(memorySize);
			allocator = new Memory::FreeListAllocator(memoryPtr, memorySize);
			block = new MockMemory(*allocator);
		}

		void TearDown()
		{
			free(memoryPtr);
		}
	};

	TEST_F(ListTest, CanCreateList)
	{
		auto list = new Utils::List<FooMock>(getMemory());

		EXPECT_NE(nullptr, list);

		delete list;
	}

	TEST_F(ListTest, CanCreateListWithCapacity)
	{
		auto list = new Utils::List<FooMock>(getMemory(), listCapacity);

		EXPECT_NE(nullptr, list);
		EXPECT_EQ(list->capacity(), listCapacity);
		EXPECT_EQ(sizeof(FooMock) * listCapacity, allocator->getAllocationSize(list->begin()));

		delete list;
	}

	TEST_F(ListTest, CanAddItemToList)
	{
		auto list = new Utils::List<FooMock>(getMemory(), listCapacity);

		list->emplace();

		EXPECT_EQ(1, list->size());
		EXPECT_EQ(sizeof(FooMock) * list->capacity(), allocator->getAllocationSize(list->begin()));

		delete list;
	}

	TEST_F(ListTest, CanRemoveItemFromList)
	{
		auto list = new Utils::List<FooMock>(getMemory(), listCapacity);

		list->emplace();

		EXPECT_EQ(1, list->size());
		EXPECT_CALL((*list)[0], Die());

		list->erase(0);

		EXPECT_EQ(0, list->size());
		EXPECT_EQ(sizeof(FooMock) * list->capacity(), allocator->getAllocationSize(list->begin()));

		delete list;
	}

	TEST_F(ListTest, CanAddManyItemsToList)
	{
		auto list = new Utils::List<int>(getMemory());

		for(int i = 0; i < listCapacity; i++)
			list->add(i);

		EXPECT_EQ(list->size(), listCapacity);
		EXPECT_LE(list->size(), list->capacity());

		delete list;
	}

	TEST_F(ListTest, CanRemoveItemFromMiddleOfList)
	{
		auto list = new Utils::List<FooMock>(getMemory(), listCapacity);

		list->emplace();
		list->emplace();
		FooMock& deletedItem = list->emplace();
		list->emplace();
		list->emplace();

		EXPECT_EQ(5, list->size());
		EXPECT_CALL(deletedItem, Die());

		list->erase(2);

		for(auto i = 0; i < list->size(); i++)
		{
			uint32_t number = (*list)[1].Quack();
			EXPECT_EQ(FooMock::fooConst, number);
		}

		EXPECT_EQ(4, list->size());
		EXPECT_EQ(sizeof(FooMock) * list->capacity(), allocator->getAllocationSize(list->begin()));

		delete list;
	}
}
