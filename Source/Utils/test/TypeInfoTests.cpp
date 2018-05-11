//
// Created by mrjaqbq on 13.05.16.
//

#include <catch.hpp>
#include "Utils/TypeInfo.h"

namespace TypeInfoTests
{
	template <typename D>
	class Dummy {};

    TEST_CASE("TypeInfoTest")
    {
        SECTION("AreTypedefEqual")
        {
            auto int_id = TypeInfo<int>::id();
            auto int32_id = TypeInfo<int32_t>::id();

            REQUIRE(int_id == int32_id);
        }

        SECTION("AreAtomTypesDifferent")
        {
            auto int_id = TypeInfo<int>::id();
            auto bool_id = TypeInfo<bool>::id();

            REQUIRE(int_id != bool_id);
        }

        SECTION("AreTemplatesDifferent")
        {
            auto int_id = TypeInfo<Dummy<int>>::id();
            auto bool_id = TypeInfo<Dummy<bool>>::id();

            REQUIRE(int_id != bool_id);
        }

        SECTION("AreNamesProper")
        {
            auto name = TypeInfo<int>::name();

            REQUIRE(strcmp(name.c_str(), "int") == 0);
        }
    }
}

