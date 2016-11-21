//
// Created by bentoo on 21.11.16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Utils/TypeIndexList.h"

namespace TypeIndexListTests
{
	class TypeIndexListTest : public ::testing::Test
	{
	protected:
		using myList = Utils::TypeIndexList<int, float, double, bool>;
		using otherList = Utils::TypeIndexList<float, int>;

	public:
		void SetUp()
		{

		}

		void TearDown()
		{

		}
	};

	TEST_F(TypeIndexListTest, AreUsedTypesExisting)
	{
		EXPECT_TRUE(otherList::contains<int>());
		EXPECT_TRUE(otherList::contains<float>());
	}

	TEST_F(TypeIndexListTest, AreNotUsedTypesExisting)
	{
		EXPECT_FALSE(otherList::contains<bool>());
		EXPECT_FALSE(otherList::contains<double>());
	}

	TEST_F(TypeIndexListTest, AreProperIndicesCreated)
	{
		std::size_t int_id		= myList::indexOf<int>();
		std::size_t float_id	= myList::indexOf<float>();
		std::size_t double_id	= myList::indexOf<double>();
		std::size_t bool_id		= myList::indexOf<bool>();

		EXPECT_EQ(0, int_id);
		EXPECT_EQ(1, float_id);
		EXPECT_EQ(2, double_id);
		EXPECT_EQ(3, bool_id);
	}

	TEST_F(TypeIndexListTest, AreDifferentIndicesCreated)
	{
		std::size_t int_id		= myList::indexOf<int>();
		std::size_t float_id	= myList::indexOf<float>();
		std::size_t double_id	= myList::indexOf<double>();
		std::size_t bool_id		= myList::indexOf<bool>();

		EXPECT_NE(int_id, float_id);
		EXPECT_NE(int_id, double_id);
		EXPECT_NE(int_id, bool_id);
		EXPECT_NE(float_id, double_id);
		EXPECT_NE(float_id, bool_id);
		EXPECT_NE(double_id, bool_id);
	}

	TEST_F(TypeIndexListTest, AreIndicesDifferentPerUsage)
	{
		std::size_t int_id			= myList::indexOf<int>();
		std::size_t float_id		= myList::indexOf<float>();
		std::size_t other_int_id	= otherList::indexOf<int>();
		std::size_t other_float_id	= otherList::indexOf<float>();

		EXPECT_NE(int_id, other_int_id);
		EXPECT_NE(float_id, other_float_id);
	}
}