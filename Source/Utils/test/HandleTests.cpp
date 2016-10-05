//
// Created by mrjaqbq on 10.03.16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Utils/Handle.h"

namespace ContainerTests
{
	struct FooTrait
	{
		using ptr_t = void*;

		static auto invalid() noexcept -> ptr_t
		{
			return nullptr;
		}

		static auto close(ptr_t value) noexcept -> void
		{

		}
	};

	class HandleTest : public ::testing::Test
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

	/*TEST_F(HandleTest, CanCreateHandleUninitialized)
	{
		auto handle = Utils::Handle<FooTrait> {};

		EXPECT_EQ(handle.get(), FooTrait::invalid());
	}

	TEST_F(HandleTest, CanCreateHandleInitialized)
	{
		int value = 1;
		auto handle = Utils::Handle<FooTrait> {&value};

		EXPECT_NE(handle.get(), FooTrait::invalid());
		EXPECT_EQ((int*)handle.get(), &value);
	}*/
}
