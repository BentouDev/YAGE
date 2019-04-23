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

template <typename T>
auto genericDeleter()
{
	return std::function {
		[](void * ptr) {
			delete reinterpret_cast<T*>(ptr);
	}};
}

template <typename T>
auto make_owned()
{
	return Utils::owned_ptr<T>(genericDeleter<T>(), new T());
}

namespace OwnedPtrTests
{
    TEST_CASE("OwnedPtrTest")
    {
        class IFooMock
        {
        public:
            virtual ~IFooMock() { }
        };

		class IAMock : public IFooMock
		{

		};

        using FooMock = trompeloeil::deathwatched<IFooMock>;
        using AMock = trompeloeil::deathwatched<IAMock>;

        SECTION("CanCreateAndDestroy")
        {
            auto ptr = make_owned<FooMock>();
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
            auto ptr = make_owned<IFooMock>();
            auto borrowed = ptr.borrow();
            REQUIRE(borrowed);
            REQUIRE(borrowed.get() != nullptr);
            REQUIRE(borrowed.get() == ptr.get());
        }

        SECTION("CanInvalidate")
        {
            auto ptr = make_owned<IFooMock>();
            auto borrowed = ptr.borrow();

            ptr.destroy();

            REQUIRE(!borrowed);
            REQUIRE(borrowed.get() == nullptr);
            REQUIRE(borrowed.get() == ptr.get());
        }

        SECTION("CanReplace")
        {
            auto ptr = make_owned<IFooMock>();
            auto borrowed = ptr.borrow();

            ptr.destroy();
            ptr.reset(new IFooMock());

            REQUIRE(borrowed);
            REQUIRE(borrowed.get() != nullptr);
            REQUIRE(borrowed.get() == ptr.get());
        }

        SECTION("CanMoveBorrowed")
        {
            auto ptr = make_owned<IFooMock>();
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

		SECTION("CanBorrowBase")
		{
			auto ptr = make_owned<IFooMock>();
		}
    }
}