//
// Created by bentoo on 26.11.16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Utils/FreeListAllocator.h"
#include "Utils/Handle.h"
#include "Utils/HandleContainer.h"

namespace HandleContainerTests
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
		using object_t = MyFooMock;
		using handle_t = Utils::Handle<MyFooMock>;

		inline static void cleanUp(object_t& first)
		{

		}

		inline static void swap(object_t& first, object_t& second) noexcept
		{

		}

		inline static void incrementLiveId(Utils::Index<handle_t>& index) noexcept
		{
			index.handle.liveId++;
		}

		inline static void setIndex(Utils::Index<handle_t>& index, uint16_t i) noexcept
		{
			index.handle.index = i;
		}

		inline static uint16_t getIndex(Utils::Index<handle_t>& index) noexcept
		{
			return index.handle.index;
		}

		inline static uint16_t getIndex(handle_t _handle) noexcept
		{
			return _handle.index;
		}

		inline static void setHandle(object_t& obj, handle_t& _handle) noexcept
		{
			obj.Handle = _handle;
		}

		inline static handle_t& getHandle(object_t& obj) noexcept
		{
			return obj.Handle;
		}
	};

	class HandleContainerTest : public ::testing::Test
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
			memoryBlock = new MockMemory(* new Memory::FreeListAllocator(memory, memorySize), "ContainerBlock");
		}

		void TearDown()
		{
			free(memory);
		}
	};

	TEST_F(HandleContainerTest, CanCreateContainer)
	{
		Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>* container =
				new Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>(getMemory(), count);

		ASSERT_NE(container, nullptr);

		delete container;
	}

	TEST_F(HandleContainerTest, CanCreateObject)
	{
		Utils::HandleContainer<FooTrait, Utils::RawHandle, 1> container(getMemory(), count);
		FooTrait::handle_t handle = container.create(13);

		ASSERT_NE(handle, FooTrait::handle_t::invalid());
		ASSERT_NE(handle.key, 0);
		ASSERT_NE(handle.liveId, 0);
		ASSERT_EQ(handle.typeId, TypeInfo<MyFooMock>::id());
		ASSERT_EQ(container.get(handle).test, 13);
		ASSERT_TRUE(container.contains(handle));
	}

	TEST_F(HandleContainerTest, CanRemoveObject)
	{
		Utils::HandleContainer<FooTrait, Utils::RawHandle, 1> container(getMemory(), count);

		auto handle = container.create();
		container.remove(handle);

		ASSERT_FALSE(container.contains(handle));
	}

	TEST_F(HandleContainerTest, CanReuseObject)
	{
		Utils::HandleContainer<FooTrait, Utils::RawHandle, 1> container(getMemory(), count);

		auto oldHandle = container.create();
		container.remove(oldHandle);
		auto newHandle = container.create();

		ASSERT_TRUE(container.contains(newHandle));
		ASSERT_FALSE(container.contains(oldHandle));
		ASSERT_NE(oldHandle.key, newHandle.key);
	}

	TEST_F(HandleContainerTest, CanFreeContainer)
	{
		Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>* container =
				new Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>(getMemory(), count);

		auto handle = container->create();
		auto& obj = container->get(handle);

		EXPECT_CALL(obj, Die()).Times(1);

		delete container;
	}

	TEST_F(HandleContainerTest, CanCreateManyItemsFromContainer)
	{
		Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>* container =
				new Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>(getMemory(), count);

		for(int i = 0; i < count; i++)
			container->create(i);

		EXPECT_EQ(container->size(), count);
	}

	TEST_F(HandleContainerTest, CanRemoveItemFromMiddleOfContainer)
	{
		const int elementCount = 5;
		const int elementToDelete = 2;

		Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>* container =
				new Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>(getMemory(), count);

		std::vector<FooTrait::handle_t> handles;

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
