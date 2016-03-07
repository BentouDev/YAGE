//
// Created by mrjaqbq on 06.03.16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/Container.h"

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
		auto const count = 32;
		auto container = new Utils::StaticContainer<FooMock, count>;

		EXPECT_NE(container, nullptr);

		delete container;
	}
}