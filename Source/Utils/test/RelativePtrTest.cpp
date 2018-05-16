//
// Created by bentoo on 14.05.18.
//

#include <catch.hpp>
#include <Utils/RelativePtr.h>

namespace RelativePtrTest
{
    struct Node {
        std::int32_t value;
        Utils::relative_ptr<Node> next;
    };

    static_assert(sizeof(Node) == 8, "Node must be 8 bytes!");

    TEST_CASE("RelativePtrTest")
    {
        SECTION("IsNullByDefault")
        {
            Utils::relative_ptr<Node> ptr;

            REQUIRE(ptr == nullptr);
            REQUIRE(!ptr);
        }

        SECTION("CanBeAssignedFromPtr")
        {
            Utils::relative_ptr<Node> ptr;
            Node node;
            auto* raw_ptr = &node;
            ptr = raw_ptr;

            REQUIRE(ptr);
            REQUIRE(ptr != nullptr);
            REQUIRE((&ptr)->operator->() == raw_ptr);
            REQUIRE(ptr == raw_ptr);
        }

        SECTION("CanAssignSelf")
        {
            Utils::relative_ptr<Node> ptr;
            auto* raw_ptr = &ptr;
            ptr = raw_ptr;

            REQUIRE(ptr);
            REQUIRE(ptr != nullptr);
            REQUIRE(ptr == raw_ptr);
            REQUIRE(ptr == &ptr);
        }

        SECTION("CanPointAnywhere")
        {
            Utils::relative_ptr<Node, std::int64_t> ptr;
            ptr = new Node();
            ptr->next = new Node();

            REQUIRE(ptr);
            REQUIRE(ptr != nullptr);
            REQUIRE(ptr->next);
            REQUIRE(ptr->next != nullptr);
            // REQUIRE(ptr->next != ptr);
        }
    }
}
