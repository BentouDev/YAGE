//
// Created by mrjaqbq on 06.03.16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Handle.h>
#include "../include/Container.h"

namespace MemoryTests
{
	class FooMock;

	class FooMock
	{
	public:
		FooMock() {}
		FooMock(unsigned t) : test(t) {}

		Utils::Handle<FooMock> Handle;
		unsigned test;

		auto swap(FooMock& other) -> void { }
		auto cleanUp() -> void { }
	};

	class FooTrait
	{
	public:
		using type = FooMock;
		using handle = Utils::Handle<FooMock>;

		inline static void cleanUp(type& first)
		{
			first.cleanUp();
		}

		inline static void swap(type& first, type& second) noexcept
		{
			first.swap(second);
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

	TEST_F(ContainerTest, CanCreateContainer)
	{
		unsigned count = 32;
		Utils::Container<FooTrait>* container = new Utils::Container<FooTrait>(count);

		ASSERT_NE(container, nullptr);

		delete container;
	}

	TEST_F(ContainerTest, CanCreateObject)
	{
		Utils::Container<FooTrait> container(32);
		auto handle = container.create(13);

		ASSERT_NE(handle.key, 0);
		ASSERT_NE(handle.liveId, 0);
		ASSERT_EQ(handle.typeId, TypeInfo<FooMock>::id());
		ASSERT_EQ(container[handle].test, 13);
		ASSERT_TRUE(container.contains(handle));
	}

	TEST_F(ContainerTest, CanRemoveObject)
	{
		Utils::Container<FooTrait> container(32);
		auto handle = container.create();
		container.remove(handle);

		ASSERT_FALSE(container.contains(handle));
	}

	TEST_F(ContainerTest, CanReuseObject)
	{
		Utils::Container<FooTrait> container(32);

		auto oldHandle = container.create();
		container.remove(oldHandle);
		auto newHandle = container.create();

		ASSERT_TRUE(container.contains(newHandle));
		ASSERT_FALSE(container.contains(oldHandle));
		ASSERT_NE(oldHandle.key, newHandle.key);
	}
}
