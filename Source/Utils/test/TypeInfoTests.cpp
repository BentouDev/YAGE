//
// Created by mrjaqbq on 13.05.16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Utils/TypeInfo.h"

namespace TypeInfoTests
{
	template <typename D>
	class Dummy {};

	class TypeInfoTest : public ::testing::Test
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

	TEST_F(TypeInfoTest, AreTypedefEqual)
	{
		auto int_id = TypeInfo<int>::id();
		auto int32_id = TypeInfo<int32_t>::id();

		EXPECT_EQ(int_id, int32_id);
	}

	TEST_F(TypeInfoTest, AreAtomTypesDifferent)
	{
		auto int_id = TypeInfo<int>::id();
		auto bool_id = TypeInfo<bool>::id();

		EXPECT_NE(int_id, bool_id);
	}

	TEST_F(TypeInfoTest, AreTemplatesDifferent)
	{
		auto int_id = TypeInfo<Dummy<int>>::id();
		auto bool_id = TypeInfo<Dummy<bool>>::id();

		EXPECT_NE(int_id, bool_id);
	}

	TEST_F(TypeInfoTest, AreNamesProper)
	{
		auto name = TypeInfo<int>::name();

		EXPECT_NE(name, "int");
	}

}

