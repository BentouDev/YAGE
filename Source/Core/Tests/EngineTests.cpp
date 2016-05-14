//
// Created by mrjaqbq on 04.04.16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "DeviceMock.h"
#include "../Source/Engine.h"
#include "../Source/Window.h"

namespace EngineTests
{
	class EngineTest : public ::testing::Test
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

	TEST_F(EngineTest, CanCreateEngine)
	{
		auto name = "test";
		auto engine = new Core::Engine(name);

		ASSERT_NE(engine, nullptr);

		delete engine;
	}

	TEST_F(EngineTest, CanInitializeEngineWithApi)
	{
		auto name = "test";
		auto engine = new Core::Engine(name);
		auto mock = new DeviceMock();

		mock->InitWillSucced();

		EXPECT_CALL(*mock, initialize()).Times(1);

		auto result = engine->Initialize(Utils::borrowed_ptr<Gfx::BaseDevice>(mock));

		ASSERT_TRUE(result);

		delete engine;
		delete mock;
	}
}