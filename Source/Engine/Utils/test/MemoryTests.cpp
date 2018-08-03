//
// Created by mrjaqbq on 05.03.16.
//

#include <catch.hpp>
#include <trompeloeil.hpp>
#include <malloc.h>
#include "Utils/StackAllocator.h"
#include "Utils/SafeDelete.h"
#include "Utils/MemoryBlock.h"
#include "MockHelper.h"

extern template struct trompeloeil::reporter<trompeloeil::specialized>;

namespace MemoryTests
{
	typedef Memory::MemoryBlock<Memory::StackAllocator> MockMemory;

	class IFooMock
	{
	public:
		virtual ~IFooMock() { }
	};

	using FooMock = trompeloeil::deathwatched<IFooMock>;

    TEST_CASE("MemoryTest")
    {
        SECTION("CanSafeDelete")
        {
            auto foo = new FooMock();

            REQUIRE_DESTRUCTION((*foo));

            Memory::SafeDelete(foo);

            REQUIRE(nullptr == foo);
        }

        SECTION("CanSafeDeleteTwice")
        {
            auto foo = new FooMock();

            REQUIRE_DESTRUCTION((*foo));

            Memory::SafeDelete(foo);
            Memory::SafeDelete(foo);

            REQUIRE(nullptr == foo);
        }

        SECTION("CanSafeDeleteArrayOfObjects")
        {
            int count = 6;
            auto foos = new FooMock[count];

            //for(auto i = 0; i < size; i++)
            //	auto obj = new (&foos[i]) FooMock();

            TDestructionReqVec vec;
            for (auto i = 0; i < count; i++)
                vec.emplace_back(NAMED_REQUIRE_DESTRUCTION(foos[i]));

            Memory::SafeDeleteArray(foos);

            REQUIRE(nullptr == foos);

            vec.clear();
        }

        SECTION("CanSafeDeleteArrayOfObjectsTwice")
        {
            int count = 6;
            auto foos = new FooMock[count];

            //for(auto i = 0; i < size; i++)
            //	auto = new (foos[i]) FooMock();

            TDestructionReqVec vec;
            for (auto i = 0; i < count; i++)
                vec.emplace_back(NAMED_REQUIRE_DESTRUCTION(foos[i]));
                //REQUIRE_CALL(foos[i], Die()).TIMES(1);
                // EXPECT_CALL(foos[i], Die()).Times(1);
                // fakeit::Verify(Method(foos[i], Die)).Exactly(1);

            Memory::SafeDeleteArray(foos);
            Memory::SafeDeleteArray(foos);

            REQUIRE(nullptr == foos);

            vec.clear();
        }

        SECTION("CanSafeFree")
        {
            auto memory = (FooMock*)malloc(sizeof(FooMock));
            auto foo = new(memory) FooMock();

            REQUIRE_DESTRUCTION((*foo));

            Memory::SafeFree(foo);

            REQUIRE(nullptr == foo);
        }

        SECTION("CanSafeFreeTwice")
        {
            auto memory = (FooMock*)malloc(sizeof(FooMock));
            auto foo = new(memory) FooMock();

            REQUIRE_DESTRUCTION((*foo));

            Memory::SafeFree(foo);
            Memory::SafeFree(foo);

            REQUIRE(nullptr == foo);
        }

        SECTION("CanSafeFreeArray")
        {
            int count = 4;
            auto* memory = (FooMock*)malloc(sizeof(FooMock) * count);

            for (auto i = 0; i < count; i++)
                new (&memory[i]) FooMock();

            TDestructionReqVec vec;
            for (auto i = 0; i < count; i++)
                vec.emplace_back(NAMED_REQUIRE_DESTRUCTION(memory[i]));

            Memory::SafeFreeArray(memory, count);

            REQUIRE(nullptr == memory);

            vec.clear();
        }

        SECTION("CanSafeFreeArrayTwice")
        {
            int count = 4;
            auto* memory = (FooMock*)malloc(sizeof(FooMock) * count);

            for (auto i = 0; i < count; i++)
                new (&memory[i]) FooMock();

            TDestructionReqVec vec;
            for (auto i = 0; i < count; i++)
                vec.emplace_back(NAMED_REQUIRE_DESTRUCTION(memory[i]));

            Memory::SafeFreeArray(memory, count);
            Memory::SafeFreeArray(memory, count);

            REQUIRE(nullptr == memory);

            vec.clear();
        }

        SECTION("CanAllocateMemoryFromBlock")
        {
            const std::size_t memorySize = 1024;
            void*			  memoryPtr = malloc(memorySize);
            auto*			  allocator = new Memory::StackAllocator(memoryPtr, memorySize);

            MockMemory block(*allocator, "");
            FooMock* foo = YAGE_CREATE_NEW(block, FooMock)();

            REQUIRE(nullptr != foo);

            delete(allocator);

            free(memoryPtr);
        }

        SECTION("CanAllocateAndFreeMemoryFromBlock")
        {
            const std::size_t memorySize = 1024;
            void*			  memoryPtr = malloc(memorySize);
            FooMock* 		  foo = nullptr;
            auto*			  allocator = new Memory::StackAllocator(memoryPtr, memorySize);

            MockMemory block(*allocator, "CanAllocateAndFreeMemoryFromBlock");
            foo = YAGE_CREATE_NEW(block, FooMock)();

            REQUIRE_DESTRUCTION((*foo));

            Memory::Delete(block, foo);

            REQUIRE(nullptr == foo);

            delete allocator;

            free(memoryPtr);
        }
    }
}