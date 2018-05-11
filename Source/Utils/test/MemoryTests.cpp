//
// Created by mrjaqbq on 05.03.16.
//

#include <catch.hpp>
#include <trompeloeil.hpp>
#include <malloc.h>
#include "Utils/StackAllocator.h"
#include "Utils/SafeDelete.h"
#include "Utils/MemoryBlock.h"

extern template struct trompeloeil::reporter<trompeloeil::specialized>;

namespace MemoryTests
{
	typedef Memory::MemoryBlock<Memory::StackAllocator> MockMemory;

	class IFooMock
	{
	public:
        //MAKE_MOCK0(Die, void());

		~IFooMock()
		{
			//Die();
		}
	};

    TEST_CASE("MemoryTest")
    {
        SECTION("CanSafeDelete")
        {
            auto foo = new IFooMock();

            // EXPECT_CALL(*foo, Die());
            //REQUIRE_CALL((*foo), Die()).TIMES(1);

            Memory::SafeDelete(foo);

            // fakeit::Verify(Method((*foo), Die));

            REQUIRE(nullptr == foo);
        }

        SECTION("CanSafeDeleteTwice")
        {
            auto foo = new IFooMock();

            // EXPECT_CALL(*foo, Die()).Times(1);
            //REQUIRE_CALL((*foo), Die()).TIMES(1);

            Memory::SafeDelete(foo);
            Memory::SafeDelete(foo);

            // fakeit::Verify(Method((*foo), Die)).Exactly(1);

            REQUIRE(nullptr == foo);
        }

        SECTION("CanSafeDeleteArrayOfObjects")
        {
            int count = 6;
            auto foos = new IFooMock[count];

            //for(auto i = 0; i < size; i++)
            //	auto obj = new (&foos[i]) FooMock();

            //for (auto i = 0; i < count; i++)
                //REQUIRE_CALL(foos[i], Die()).TIMES(1);
                // EXPECT_CALL(foos[i], Die()).Times(1);
                // fakeit::Verify(Method(foos[i], Die)).Exactly(1);

            Memory::SafeDeleteArray(foos);

            REQUIRE(nullptr == foos);
        }

        SECTION("CanSafeDeleteArrayOfObjectsTwice")
        {
            int count = 6;
            auto foos = new IFooMock[count];

            //for(auto i = 0; i < size; i++)
            //	auto = new (foos[i]) FooMock();

            //for (auto i = 0; i < count; i++)
                //REQUIRE_CALL(foos[i], Die()).TIMES(1);
                // EXPECT_CALL(foos[i], Die()).Times(1);
                // fakeit::Verify(Method(foos[i], Die)).Exactly(1);

            Memory::SafeDeleteArray(foos);
            Memory::SafeDeleteArray(foos);

            REQUIRE(nullptr == foos);
        }

        SECTION("CanSafeFree")
        {
            auto memory = (IFooMock*)malloc(sizeof(IFooMock));
            auto foo = new(memory) IFooMock();

            // EXPECT_CALL(*foo, Die());
            //REQUIRE_CALL((*foo), Die()).TIMES(1);

            // fakeit::Verify(Method((*foo), Die)).Exactly(1);

            Memory::SafeFree(foo);

            REQUIRE(nullptr == foo);
        }

        SECTION("CanSafeFreeTwice")
        {
            auto memory = (IFooMock*)malloc(sizeof(IFooMock));
            auto foo = new(memory) IFooMock();

            // EXPECT_CALL(*foo, Die());
            //REQUIRE_CALL((*foo), Die()).TIMES(1);

            // fakeit::Verify(Method((*foo), Die)).Exactly(1);

            Memory::SafeFree(foo);
            Memory::SafeFree(foo);

            REQUIRE(nullptr == foo);
        }

        SECTION("CanSafeFreeArray")
        {
            int count = 4;
            IFooMock* memory = (IFooMock*)malloc(sizeof(IFooMock) * count);

            for (auto i = 0; i < count; i++)
                new (&memory[i]) IFooMock();

            //for (auto i = 0; i < count; i++)
              //  REQUIRE_CALL(memory[i], Die()).TIMES(1);
                // EXPECT_CALL(memory[i], Die()).Times(1);
                // fakeit::Verify(Method(memory[i], Die)).Exactly(1);

            Memory::SafeFreeArray(memory, count);

            REQUIRE(nullptr == memory);
        }

        SECTION("CanSafeFreeArrayTwice")
        {
            int count = 4;
            IFooMock* memory = (IFooMock*)malloc(sizeof(IFooMock) * count);

            for (auto i = 0; i < count; i++)
                new (&memory[i]) IFooMock();

            //for (auto i = 0; i < count; i++)
              //  REQUIRE_CALL(memory[i], Die()).TIMES(1);
                // EXPECT_CALL(memory[i], Die()).Times(1);
                // fakeit::Verify(Method(memory[i], Die)).Exactly(1);

            Memory::SafeFreeArray(memory, count);
            Memory::SafeFreeArray(memory, count);

            REQUIRE(nullptr == memory);
        }

        SECTION("CanAllocateMemoryFromBlock")
        {
            const std::size_t memorySize = 1024;
            void*			  memoryPtr = malloc(memorySize);
            auto*			  allocator = new Memory::StackAllocator(memoryPtr, memorySize);

            MockMemory block(*allocator, "");
            IFooMock* foo = YAGE_CREATE_NEW(block, IFooMock)();

            REQUIRE(nullptr != foo);

            delete(allocator);

            free(memoryPtr);
        }

        SECTION("CanAllocateAndFreeMemoryFromBlock")
        {
            const std::size_t memorySize = 1024;
            void*			  memoryPtr = malloc(memorySize);
            IFooMock* 		  foo = nullptr;
            auto*			  allocator = new Memory::StackAllocator(memoryPtr, memorySize);

            MockMemory block(*allocator, "CanAllocateAndFreeMemoryFromBlock");
            foo = YAGE_CREATE_NEW(block, IFooMock)();

            //REQUIRE_CALL(*foo, Die()).TIMES(1);
            // EXPECT_CALL(*foo, Die());
            // fakeit::Verify(Method((*foo), Die)).Exactly(1);

            Memory::Delete(block, foo);

            REQUIRE(nullptr == foo);

            delete allocator;

            free(memoryPtr);
        }
    }
}