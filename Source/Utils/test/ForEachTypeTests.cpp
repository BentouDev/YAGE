//
// Created by bentoo on 22.11.16.
//

#include <catch.hpp>
#include <Utils/ForEachType.h>

namespace ForEachTypeTests
{
    template <typename T>
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

    TEST_CASE("ForEachTypeTest")
    {
        SECTION("AreDerivedHandled")
        {
            bool first = matching::are_base_of<base>();

            REQUIRE(first);
        }

        SECTION("AreNotDerivedHandled")
        {
            bool first  = notMatching1::are_base_of<base>();
            bool second = notMatching2::are_base_of<base>();
            bool third  = notMatching3::are_base_of<base>();

            REQUIRE(!first);
            REQUIRE(!second);
            REQUIRE(!third);
        }
    }
}
