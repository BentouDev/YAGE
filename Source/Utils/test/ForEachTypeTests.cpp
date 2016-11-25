//
// Created by bentoo on 22.11.16.
//

#include <gtest/gtest.h>
#include <Utils/ForEachType.h>

namespace ForEachTypeTests
{
	class ForEachTypeTest : public ::testing::Test
	{
	protected:
		template <typename>
		struct crtp {};

		struct kwas : public crtp<kwas> {};

		struct base {};

		struct derive : public base {};

		struct koza : public base {};

		struct foo {};

		using matching = Utils::ForEachType<derive, koza>;
		using notMatching1 = Utils::ForEachType<derive, foo>;
		using notMatching2 = Utils::ForEachType<foo, derive>;
		using notMatching3 = Utils::ForEachType<derive, derive, int>;

	public:
		void SetUp()
		{

		}

		void TearDown()
		{

		}
	};

	TEST_F(ForEachTypeTest, AreDerivedHandled)
	{
		bool first = matching::are_base_of<base>();

		EXPECT_TRUE(first);
	}

	TEST_F(ForEachTypeTest, AreNotDerivedHandled)
	{
		bool first	= notMatching1::are_base_of<base>();
		bool second	= notMatching2::are_base_of<base>();
		bool third	= notMatching3::are_base_of<base>();

		EXPECT_FALSE(first);
		EXPECT_FALSE(second);
		EXPECT_FALSE(third);
	}
}
