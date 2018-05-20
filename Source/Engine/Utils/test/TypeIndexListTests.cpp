//
// Created by bentoo on 21.11.16.
//

#include <catch.hpp>
#include "Utils/TypeIndexList.h"

namespace TypeIndexListTests
{
    TEST_CASE("TypeIndexListTest")
    {
        using myList = Utils::TypeIndexList<int, float, double, bool>;
        using otherList = Utils::TypeIndexList<float, int>;

        SECTION("AreUsedTypesExisting")
        {
            REQUIRE(otherList::contains<int>());
            REQUIRE(otherList::contains<float>());
        }

        SECTION("AreNotUsedTypesExisting")
        {
            REQUIRE(!otherList::contains<bool>());
            REQUIRE(!otherList::contains<double>());
        }

        SECTION("AreProperIndicesCreated")
        {
            std::size_t int_id = myList::indexOf<int>();
            std::size_t float_id = myList::indexOf<float>();
            std::size_t double_id = myList::indexOf<double>();
            std::size_t bool_id = myList::indexOf<bool>();

            REQUIRE(0 == int_id);
            REQUIRE(1 == float_id);
            REQUIRE(2 == double_id);
            REQUIRE(3 == bool_id);
        }

        SECTION("AreDifferentIndicesCreated")
        {
            std::size_t int_id = myList::indexOf<int>();
            std::size_t float_id = myList::indexOf<float>();
            std::size_t double_id = myList::indexOf<double>();
            std::size_t bool_id = myList::indexOf<bool>();

            REQUIRE(int_id != float_id);
            REQUIRE(int_id != double_id);
            REQUIRE(int_id != bool_id);
            REQUIRE(float_id != double_id);
            REQUIRE(float_id != bool_id);
            REQUIRE(double_id != bool_id);
        }

        SECTION("AreIndicesDifferentPerUsage")
        {
            std::size_t int_id = myList::indexOf<int>();
            std::size_t float_id = myList::indexOf<float>();
            std::size_t other_int_id = otherList::indexOf<int>();
            std::size_t other_float_id = otherList::indexOf<float>();

            REQUIRE(int_id != other_int_id);
            REQUIRE(float_id != other_float_id);
        }
    }
}