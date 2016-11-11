//
// Created by mrjaqbq on 06.03.16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Utils/FreeListAllocator.h"
#include "Utils/Handle.h"
#include "Utils/Container.h"

namespace MemoryTests
{
	typedef Memory::MemoryBlock<Memory::FreeListAllocator> MockMemory;

	class MyFooMock
	{
	public:
		MOCK_METHOD0(Die, void());
		MOCK_METHOD0(Foo, void());

		virtual ~MyFooMock() { test = 0; Die(); }
		MyFooMock() : Handle(), test(fooConst) {}
		explicit MyFooMock(unsigned t) : Handle(), test(t) {}

		MyFooMock(MyFooMock&& other) : Handle(), test(other.test) { }

		MyFooMock(const MyFooMock&) = delete;
		MyFooMock& operator=(const MyFooMock&) = delete;
		MyFooMock& operator=(MyFooMock&&) = delete;

		Utils::Handle<MyFooMock> Handle;
		unsigned test;

		static const uint32_t fooConst;

		uint32_t Quack()
		{
			Foo();
			return test;
		}

	};

	const uint32_t MyFooMock::fooConst = 0xF00BACEC;

	class FooTrait
	{
	public:
		using type = MyFooMock;
		using handle = Utils::Handle<MyFooMock>;

		inline static void cleanUp(type& first)
		{

		}

		inline static void swap(type& first, type& second) noexcept
		{

		}

		inline static void incrementLiveId(Utils::Index<handle>& index) noexcept
		{
			index.handle.liveId++;
		}

		inline static void setIndex(Utils::Index<handle>& index, uint16_t i) noexcept
		{
			index.handle.index = i;
		}

		inline static uint16_t getIndex(Utils::Index<handle>& index) noexcept
		{
			return index.handle.index;
		}

		inline static uint16_t getIndex(handle _handle) noexcept
		{
			return _handle.index;
		}

		inline static void setHandle(type& obj, handle& _handle) noexcept
		{
			obj.Handle = _handle;
		}

		inline static handle& getHandle(type& obj) noexcept
		{
			return obj.Handle;
		}
	};

	class ContainerTest : public ::testing::Test
	{
	public:
		MockMemory* memoryBlock;
		unsigned memorySize = 4096 * 2;
		unsigned count = 32;
		void* memory;

		MockMemory& getMemory() const
		{
			return *memoryBlock;
		}

		void SetUp()
		{
			memory = malloc(memorySize);
			memoryBlock = new MockMemory(* new Memory::FreeListAllocator(memory, memorySize));
		}

		void TearDown()
		{
			free(memory);
		}
	};

	TEST_F(ContainerTest, CanCreateContainer)
	{
		Utils::Container<FooTrait>* container = new Utils::Container<FooTrait>(getMemory(), count);

		ASSERT_NE(container, nullptr);

		delete container;
	}

	TEST_F(ContainerTest, CanCreateObject)
	{
		Utils::Container<FooTrait> container(getMemory(), count);
		FooTrait::handle handle = container.create(13);

		ASSERT_NE(handle, FooTrait::handle::invalid());
		ASSERT_NE(handle.key, 0);
		ASSERT_NE(handle.liveId, 0);
		ASSERT_EQ(handle.typeId, TypeInfo<MyFooMock>::id());
		ASSERT_EQ(container[handle].test, 13);
		ASSERT_TRUE(container.contains(handle));
	}

	TEST_F(ContainerTest, CanRemoveObject)
	{
		Utils::Container<FooTrait> container(getMemory(), count);
		auto handle = container.create();
		container.remove(handle);

		ASSERT_FALSE(container.contains(handle));
	}

	TEST_F(ContainerTest, CanReuseObject)
	{
		Utils::Container<FooTrait> container(getMemory(), count);

		auto oldHandle = container.create();
		container.remove(oldHandle);
		auto newHandle = container.create();

		ASSERT_TRUE(container.contains(newHandle));
		ASSERT_FALSE(container.contains(oldHandle));
		ASSERT_NE(oldHandle.key, newHandle.key);
	}

	TEST_F(ContainerTest, CanFreeContainer)
	{
		Utils::Container<FooTrait>* container = new Utils::Container<FooTrait>(getMemory(), count);
		auto handle = container->create();
		auto& obj = container->get(handle);

		EXPECT_CALL(obj, Die()).Times(1);

		delete container;
	}

	TEST_F(ContainerTest, CanCreateManyItemsFromContainer)
	{
		Utils::Container<FooTrait>* container = new Utils::Container<FooTrait>(getMemory(), count);

		for(int i = 0; i < count; i++)
			container->create(i);

		EXPECT_EQ(container->size(), count);
	}

	TEST_F(ContainerTest, CanRemoveItemFromMiddleOfContainer)
	{
		const int elementCount = 5;
		const int elementToDelete = 2;
		Utils::Container<FooTrait>* container = new Utils::Container<FooTrait>(getMemory(), count);

		std::vector<FooTrait::handle> handles;

		for(auto i = 0; i < elementCount; i++)
			handles.push_back(container->create());

		MyFooMock& deletedItem = container->get(handles[elementToDelete]);

		EXPECT_EQ(elementCount, container->size());
		EXPECT_CALL(deletedItem, Die());

		container->remove(handles[elementToDelete]);

		for(auto i = 0; i < elementCount; i++)
		{
			if(i != elementToDelete)
			{
				MyFooMock& element = container->get(handles[i]);
				EXPECT_EQ(MyFooMock::fooConst, element.test);
				uint32_t number = element.Quack();
				EXPECT_EQ(MyFooMock::fooConst, number);
			}
		}

		EXPECT_EQ(elementCount - 1, container->size());
	}
}
