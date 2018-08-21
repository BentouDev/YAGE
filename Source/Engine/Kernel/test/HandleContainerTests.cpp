//
// Created by bentoo on 26.11.16.
//

#include <catch.hpp>
#include <trompeloeil.hpp>
#include <Utils/DefaultTrait.h>

#include "Utils/FreeListAllocator.h"
#include "Utils/Handle.h"
#include "Utils/HandleContainer.h"
#include "Utils/ScopeGuard.h"
#include "MockHelper.h"

extern template struct trompeloeil::reporter<trompeloeil::specialized>;

namespace HandleContainerTests
{
    typedef Memory::MemoryBlock<Memory::FreeListAllocator> MockMemory;

    class IFooMock
    {
    public:
        virtual ~IFooMock() { test = 0; /*Die()*/; }
        IFooMock() : Handle(), test(fooConst) {}
        explicit IFooMock(unsigned t) : Handle(), test(t) {}

        IFooMock(IFooMock&& other) : Handle(), test(other.test) { }

        IFooMock(const IFooMock&) = delete;
        IFooMock& operator=(const IFooMock&) = delete;
        IFooMock& operator=(IFooMock&&) = delete;

        Utils::Handle<IFooMock> Handle;
        unsigned test;

        static const uint32_t fooConst;

        uint32_t Quack()
        { return test; }
    };

    const uint32_t IFooMock::fooConst = 0xF00BACEC;

    using FooTrait        = Utils::DefaultTrait<IFooMock>;
    using DestructedMock  = trompeloeil::deathwatched<IFooMock>;
    using DestructedTrait = Utils::DefaultTrait<DestructedMock, Utils::Handle<IFooMock>>;

    TEST_CASE("HandleContainerTest")
    {
        MockMemory* memoryBlock;
        unsigned memorySize = 4096 * 2;
        unsigned count = 32;
        void* memory;

        auto getMemory = [&]() -> MockMemory&
        {
            return *memoryBlock;
        };

        memory = malloc(memorySize);
        memoryBlock = new MockMemory(*new Memory::FreeListAllocator(memory, memorySize), "ContainerBlock");

        YAGE_DISPOSE
        {
            free(memory);
        };

        SECTION("CanCreateContainer")
        {
            Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>* container =
                new Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>(getMemory(), count);

            REQUIRE(container != nullptr);

            delete container;
        }

        SECTION("CanCreateObject")
        {
            Utils::HandleContainer<FooTrait, Utils::RawHandle, 1> container(getMemory(), count);
            FooTrait::handle_t handle = container.create(13);

            REQUIRE(handle != FooTrait::handle_t::invalid());
            REQUIRE(handle.key != 0);
            REQUIRE(handle.liveId != 0);
            // ToDo : implement typeId
            // REQUIRE(handle.typeId == TypeInfo<IFooMock>::id());
            REQUIRE(container.get(handle).test == 13);
            REQUIRE(container.contains(handle));
        }

        SECTION("CanRemoveObject")
        {
            Utils::HandleContainer<DestructedTrait, Utils::RawHandle, 1> container(getMemory(), count);

            auto handle = container.create();

            REQUIRE_DESTRUCTION(container.get(handle));

            container.remove(handle);

            REQUIRE(!container.contains(handle));
        }

        SECTION("CanReuseObject")
        {
            Utils::HandleContainer<DestructedTrait, Utils::RawHandle, 1> container(getMemory(), count);

            auto oldHandle = container.create();

            REQUIRE_DESTRUCTION(container.get(oldHandle));

            container.remove(oldHandle);

            auto newHandle = container.create();

            REQUIRE(container.contains(newHandle));
            REQUIRE(!container.contains(oldHandle));
            REQUIRE(oldHandle.key != newHandle.key);

            REQUIRE_DESTRUCTION(container.get(newHandle));

            container.clear();
        }

        SECTION("CanFreeContainer")
        {
            auto* container = new Utils::HandleContainer<DestructedTrait, Utils::RawHandle, 1>(getMemory(), count);

            auto handle = container->create();
            auto& obj = container->get(handle);

            REQUIRE_DESTRUCTION(obj);

            delete container;
        }

        SECTION("CanCreateManyItemsFromContainer")
        {
            auto* container = new Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>(getMemory(), count);

            for (int i = 0; i < count; i++)
                container->create();

            REQUIRE(container->size() == count);
        }

        SECTION("CanRemoveItemFromMiddleOfContainer")
        {
            const int elementCount = 5;
            const int elementToDelete = 2;

            auto* container = new Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>(getMemory(), count);

            std::vector<DestructedTrait::handle_t> handles;

            for (auto i = 0; i < elementCount; i++)
                handles.push_back(container->create());

            auto& deletedItem = container->get(handles[elementToDelete]);

            REQUIRE(elementCount == container->size());
            //REQUIRE_DESTRUCTION(deletedItem);

            container->remove(handles[elementToDelete]);

            TDestructionReqVec vec;
            for (auto i = 0; i < elementCount; i++)
            {
                if (i != elementToDelete)
                {
                    auto& element = container->get(handles[i]);
                    REQUIRE(IFooMock::fooConst == element.test);
                    uint32_t number = element.Quack();
                    REQUIRE(IFooMock::fooConst == number);

                    //vec.emplace_back(NAMED_REQUIRE_DESTRUCTION(element));
                }
            }

            REQUIRE(elementCount - 1 == container->size());

            delete container;

            vec.clear();
        }
    }
}
