//
// Created by bentoo on 26.11.16.
//

#include <catch.hpp>
#include <trompeloeil.hpp>

#include "Utils/FreeListAllocator.h"
#include "Utils/Handle.h"
#include "Utils/HandleContainer.h"
#include "Utils/ScopeGuard.h"

extern template struct trompeloeil::reporter<trompeloeil::specialized>;

namespace HandleContainerTests
{
    typedef Memory::MemoryBlock<Memory::FreeListAllocator> MockMemory;

    class IFooMock
    {
    public:
        MAKE_MOCK0(Die, void());
        MAKE_MOCK0(Foo, void());

        virtual ~IFooMock() { test = 0; Die(); }
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
        {
            Foo();
            return test;
        }

    };

    const uint32_t IFooMock::fooConst = 0xF00BACEC;

    class FooTrait
    {
    public:
        using object_t = IFooMock;
        using handle_t = Utils::Handle<IFooMock>;

        inline static void cleanUp(object_t& first)
        {

        }

        inline static void swap(object_t& first, object_t& second) noexcept
        {

        }

        inline static void incrementLiveId(Utils::Index<handle_t>& index) noexcept
        {
            index.handle.liveId++;
        }

        inline static void setIndex(Utils::Index<handle_t>& index, uint16_t i) noexcept
        {
            index.handle.index = i;
        }

        inline static uint16_t getIndex(Utils::Index<handle_t>& index) noexcept
        {
            return index.handle.index;
        }

        inline static uint16_t getIndex(handle_t _handle) noexcept
        {
            return _handle.index;
        }

        inline static void setHandle(object_t& obj, handle_t& _handle) noexcept
        {
            obj.Handle = _handle;
        }

        inline static handle_t& getHandle(object_t& obj) noexcept
        {
            return obj.Handle;
        }
    };

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
            FooTrait::handle_t handle = container.create();

            REQUIRE(handle != FooTrait::handle_t::invalid());
            REQUIRE(handle.key != 0);
            REQUIRE(handle.liveId != 0);
            REQUIRE(handle.typeId == TypeInfo<IFooMock>::id());
            REQUIRE(container.get(handle).test == 13);
            REQUIRE(container.contains(handle));
        }

        SECTION("CanRemoveObject")
        {
            Utils::HandleContainer<FooTrait, Utils::RawHandle, 1> container(getMemory(), count);

            auto handle = container.create();
            container.remove(handle);

            REQUIRE(!container.contains(handle));
        }

        SECTION("CanReuseObject")
        {
            Utils::HandleContainer<FooTrait, Utils::RawHandle, 1> container(getMemory(), count);

            auto oldHandle = container.create();
            container.remove(oldHandle);
            auto newHandle = container.create();

            REQUIRE(container.contains(newHandle));
            REQUIRE(!container.contains(oldHandle));
            REQUIRE(oldHandle.key != newHandle.key);
        }

        SECTION("CanFreeContainer")
        {
            Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>* container =
                new Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>(getMemory(), count);

            auto handle = container->create();
            auto& obj = container->get(handle);

            // EXPECT_CALL(obj, Die()).Times(1);
            REQUIRE_CALL(obj, Die()).TIMES(1);

            delete container;

            // fakeit::Verify(Method(obj, Die));
        }

        SECTION("CanCreateManyItemsFromContainer")
        {
            Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>* container =
                new Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>(getMemory(), count);

            for (int i = 0; i < count; i++)
                container->create();

            REQUIRE(container->size() == count);
        }

        SECTION("CanRemoveItemFromMiddleOfContainer")
        {
            const int elementCount = 5;
            const int elementToDelete = 2;

            Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>* container =
                new Utils::HandleContainer<FooTrait, Utils::RawHandle, 1>(getMemory(), count);

            std::vector<FooTrait::handle_t> handles;

            for (auto i = 0; i < elementCount; i++)
                handles.push_back(container->create());

            IFooMock& deletedItem = container->get(handles[elementToDelete]);

            REQUIRE(elementCount == container->size());
            REQUIRE_CALL(deletedItem, Die()).TIMES(1);

            container->remove(handles[elementToDelete]);

            // fakeit::Verify(Method(deletedItem, Die));

            for (auto i = 0; i < elementCount; i++)
            {
                if (i != elementToDelete)
                {
                    IFooMock& element = container->get(handles[i]);
                    REQUIRE(IFooMock::fooConst == element.test);
                    uint32_t number = element.Quack();
                    REQUIRE(IFooMock::fooConst == number);
                }
            }

            REQUIRE(elementCount - 1 == container->size());
        }
    }
}
