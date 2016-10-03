//
// Created by bentoo on 10/1/16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <malloc.h>
#include <FreeListAllocator.h>
#include "../include/List.h"

namespace ListTests
{
	typedef Memory::MemoryBlock<Memory::FreeListAllocator> MockMemory;

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
		void SetUp()
		{

		}

		void TearDown()
		{

		}
	};

	TEST_F(ListTest, CanCreateList)
	{
		const std::size_t memorySize = 1024;
		void*			  memoryPtr  = malloc(memorySize);
		MockMemory 		  block(*new Memory::FreeListAllocator(memoryPtr, memorySize));

		auto list = new Utils::List<FooMock>(block);

		EXPECT_NE(nullptr, list);

		free(memoryPtr);
	}

	TEST_F(ListTest, CanCreateListWithCapacity)
	{
		const std::size_t listCapacity = 4;
		const std::size_t memorySize = 1024;
		void*			  memoryPtr  = malloc(memorySize);

		Memory::FreeListAllocator* allocator = new Memory::FreeListAllocator(memoryPtr, memorySize);

		MockMemory block(*allocator);

		auto list = new Utils::List<FooMock>(block, listCapacity);

		EXPECT_NE(nullptr, list);
		EXPECT_EQ(list->capacity(), listCapacity);
		EXPECT_EQ(sizeof(FooMock) * listCapacity, allocator->getAllocationSize(list->begin()));

		free(memoryPtr);
	}

	TEST_F(ListTest, CanAddItemToList)
	{
		const std::size_t listCapacity = 4;
		const std::size_t memorySize = 1024;
		void*			  memoryPtr  = malloc(memorySize);

		Memory::FreeListAllocator* allocator = new Memory::FreeListAllocator(memoryPtr, memorySize);

		MockMemory block(*allocator);

		auto list = new Utils::List<FooMock>(block, listCapacity);

		list->add();

		EXPECT_EQ(1, list->size());
		EXPECT_EQ(sizeof(FooMock) * list->capacity(), allocator->getAllocationSize(list->begin()));

		free(memoryPtr);
	}

	TEST_F(ListTest, CanRemoveItemFromList)
	{
		const std::size_t listCapacity = 4;
		const std::size_t memorySize = 1024;
		void*			  memoryPtr  = malloc(memorySize);

		Memory::FreeListAllocator* allocator = new Memory::FreeListAllocator(memoryPtr, memorySize);

		MockMemory block(*allocator);

		auto list = new Utils::List<FooMock>(block, listCapacity);

		list->add();

		EXPECT_EQ(1, list->size());
		EXPECT_CALL((*list)[0], Die());

		list->erase(0);

		EXPECT_EQ(0, list->size());
		EXPECT_EQ(sizeof(FooMock) * list->capacity(), allocator->getAllocationSize(list->begin()));

		free(memoryPtr);
	}

	TEST_F(ListTest, CanAddManyItemsToList)
	{
		const std::size_t listCapacity 	= 512;
		const std::size_t memorySize 	= 65535;
		void*			  memoryPtr  	= malloc(memorySize);

		Memory::FreeListAllocator* allocator = new Memory::FreeListAllocator(memoryPtr, memorySize);

		MockMemory block(*allocator);

		auto list = new Utils::List<int>(block);

		for(int i = 0; i < listCapacity; i++)
			list->add(i);

		EXPECT_EQ(list->size(), listCapacity);
		EXPECT_LE(list->size(), list->capacity());

		free(memoryPtr);
	}

	TEST_F(ListTest, CanRemoveItemFromMiddleOfList)
	{
		const std::size_t listCapacity = 4;
		const std::size_t memorySize = 4096;
		void*			  memoryPtr  = malloc(memorySize);

		Memory::FreeListAllocator* allocator = new Memory::FreeListAllocator(memoryPtr, memorySize);

		MockMemory block(*allocator);

		auto list = new Utils::List<FooMock>(block, listCapacity);

		list->add();
		list->add();
		list->add();
		list->add();
		list->add();

		EXPECT_EQ(5, list->size());
		EXPECT_CALL((*list)[2], Die());

		list->erase(2);

		for(auto i = 0; i < list->size(); i++)
		{
			uint32_t number = (*list)[1].Quack();
			EXPECT_EQ(FooMock::fooConst, number);
		}

		EXPECT_EQ(4, list->size());
		EXPECT_EQ(sizeof(FooMock) * list->capacity(), allocator->getAllocationSize(list->begin()));

		free(memoryPtr);
	}
}
