//
// Created by bentoo on 10/1/16.
//

#include <catch.hpp>
#include <trompeloeil.hpp>
#include <malloc.h>

#include "Utils/FreeListAllocator.h"
#include "Utils/List.h"
#include "Utils/ScopeGuard.h"

#ifdef YAGE_VALGRIND
#include "Utils/ValgrindMemoryBoundChecker.h"
#include "Utils/ValgrindMemoryTracker.h"
#else
#include "Utils/SimpleMemoryBoundChecker.h"
#include "Utils/SimpleMemoryTracker.h"
#endif

extern template struct trompeloeil::reporter<trompeloeil::specialized>;

namespace ListTests
{
#ifdef YAGE_VALGRIND
	typedef Memory::MemoryBlock
			<Memory::FreeListAllocator,
			 Memory::ValgrindMemoryBoundChecker,
			 Memory::ValgrindMemoryTracker> MockMemory;
#else
	typedef Memory::MemoryBlock <
			Memory::FreeListAllocator,
			Memory::SimpleMemoryBoundChecker,
			Memory::SimpleMemoryTracker> MockMemory;
#endif

	class IFooMock
	{
		uint32_t magicNumber;

	public:
        MAKE_MOCK0(Die, void());
        MAKE_MOCK0(Foo, void());

		static const uint32_t fooConst;

		uint32_t Quack()
		{
			Foo();
			return magicNumber;
		}

        IFooMock()
		{
			magicNumber = fooConst;
		}

		~IFooMock()
		{
			Die();
		}
	};

	const uint32_t IFooMock::fooConst = 0xF00BACEC;

    TEST_CASE("ListTest")
    {
        const std::size_t listCapacity = 4;
        const std::size_t memorySize = 4096;
        void*			  memoryPtr;

        Memory::FreeListAllocator* allocator;

        MockMemory* block;

        auto getMemory = [&]() -> MockMemory&
        {
            return *block;
        };

        memoryPtr = malloc(memorySize);
        allocator = new Memory::FreeListAllocator(memoryPtr, memorySize);
        block = new MockMemory(*allocator, "ListTests");

        YAGE_DISPOSE
        {
            free(memoryPtr);
        };

        SECTION("CanCreateList")
        {
            auto list = new Utils::List<IFooMock>(getMemory());

            REQUIRE(nullptr != list);

            delete list;
        }

        SECTION("CanCreateListWithCapacity")
        {
            auto list = new Utils::List<IFooMock>(getMemory(), listCapacity);

            REQUIRE(nullptr != list);
            REQUIRE(list->capacity() == listCapacity);
            REQUIRE(sizeof(IFooMock) * listCapacity == getMemory().getAllocationSize(list->begin()));

            delete list;
        }

        SECTION("CanAddItemToList")
        {
            auto list = new Utils::List<IFooMock>(getMemory(), listCapacity);

            list->emplace();

            REQUIRE(1 == list->size());
            REQUIRE(sizeof(IFooMock) * list->capacity() == getMemory().getAllocationSize(list->begin()));

            delete list;
        }

        SECTION("CanAddItemToListWithCapacity")
        {
            auto list = new Utils::List<IFooMock>(getMemory());

            list->reserve(1);
            list->emplace();

            REQUIRE(1 == list->size());
            REQUIRE(1 == list->capacity());
            REQUIRE(sizeof(IFooMock) * list->capacity() == getMemory().getAllocationSize(list->begin()));

            delete list;
        }

        SECTION("CanRemoveItemFromList")
        {
            auto list = new Utils::List<IFooMock>(getMemory(), listCapacity);

            list->emplace();

            REQUIRE(1 == list->size());
            REQUIRE_CALL((*list)[0], Die()).TIMES(1);
            // EXPECT_CALL((*list)[0], Die());

            list->eraseAt(0);

            // fakeit::Verify(Method((*list)[0], Die));

            REQUIRE(0 == list->size());
            REQUIRE(sizeof(IFooMock) * list->capacity() == getMemory().getAllocationSize(list->begin()));

            delete list;
        }

        SECTION("CanAddManyItemsToList")
        {
            auto list = new Utils::List<int>(getMemory());

            for (int i = 0; i < listCapacity; i++)
                list->add(i);

            REQUIRE(list->size() == listCapacity);
            REQUIRE(list->size() <= list->capacity());

            delete list;
        }

        SECTION("CanRemoveItemFromMiddleOfList")
        {
            auto list = new Utils::List<IFooMock>(getMemory(), listCapacity);

            list->emplace();
            list->emplace();
            IFooMock& deletedItem = list->emplace();
            list->emplace();
            list->emplace();

            REQUIRE(5 == list->size());

            list->eraseAt(2);

            for (auto i = 0; i < list->size(); i++)
            {
                uint32_t number = (*list)[i].Quack();
                REQUIRE(IFooMock::fooConst == number);
            }

            REQUIRE(4 == list->size());
            REQUIRE(sizeof(IFooMock) * list->capacity() == getMemory().getAllocationSize(list->begin()));

            // ::testing::Mock::VerifyAndClearExpectations(&deletedItem);

            delete list;
        }
    }
}
