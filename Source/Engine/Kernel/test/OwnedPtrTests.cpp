#include <catch.hpp>
#include <trompeloeil.hpp>
#include <malloc.h>
#include "ExternalOwnedPtr.h"
#include "Utils/StackAllocator.h"
#include "Utils/SafeDelete.h"
#include "Utils/MemoryBlock.h"
#include "MockHelper.h"

extern template struct trompeloeil::reporter<trompeloeil::specialized>;

struct ExternMockType
{

};

Sth::~Sth()
{ }

namespace OwnedPtrTests
{
    TEST_CASE("OwnedPtrTest")
    {
        class IFooMock
        {
        public:
            virtual ~IFooMock() { }
        };

        using FooMock = trompeloeil::deathwatched<IFooMock>;

        SECTION("CanCreateAndDestroy")
        {
            auto ptr = Utils::owned_ptr<FooMock>(new FooMock());
            REQUIRE(ptr);
            REQUIRE(ptr.get() != nullptr);

            REQUIRE_DESTRUCTION(*ptr.get());
            ptr.destroy();
            REQUIRE(ptr.get() == nullptr);
        }

        // SECTION("CanCreateFromBlock")
        // {
        //     { // inline block
        //         auto ptr = Utils::make_owned<FooMock>(Memory::GetDefaultBlock<Utils::owned_ptr<FooMock>>());
        //         REQUIRE_DESTRUCTION(*ptr.get());
        //         ptr.~owned_ptr();
        //     }
        // }

        SECTION("CanBorrow")
        {
            auto ptr = Utils::owned_ptr<IFooMock>(new IFooMock());
            auto borrowed = ptr.borrow();
            REQUIRE(borrowed);
            REQUIRE(borrowed.get() != nullptr);
            REQUIRE(borrowed.get() == ptr.get());
        }

        SECTION("CanInvalidate")
        {
            auto ptr = Utils::owned_ptr<IFooMock>(new IFooMock());
            auto borrowed = ptr.borrow();

            ptr.destroy();

            REQUIRE(!borrowed);
            REQUIRE(borrowed.get() == nullptr);
            REQUIRE(borrowed.get() == ptr.get());
        }

        SECTION("CanReplace")
        {
            auto ptr = Utils::owned_ptr<IFooMock>(new IFooMock());
            auto borrowed = ptr.borrow();

            ptr.destroy();
            ptr.reset(new IFooMock());

            REQUIRE(borrowed);
            REQUIRE(borrowed.get() != nullptr);
            REQUIRE(borrowed.get() == ptr.get());
        }

        SECTION("CanMoveBorrowed")
        {
            auto ptr = Utils::owned_ptr<IFooMock>(new IFooMock());
            {
                auto borrowed = ptr.borrow();
                {
                    auto newBorrower = std::move(borrowed);
                    REQUIRE(ptr.hasBorrowers());
                    REQUIRE(newBorrower);
                    REQUIRE(newBorrower.get() != nullptr);
                    REQUIRE(newBorrower.get() == ptr.get());
                }
            }

            REQUIRE(!ptr.hasBorrowers());
        }
    }
}