//
// Created by mrjaqbq on 06.03.16.
//

#include <catch.hpp>
#include <trompeloeil.hpp>

#include "Utils/FreeListAllocator.h"
#include "Utils/Handle.h"
#include "Utils/Container.h"
#include "Utils/ScopeGuard.h"
#include "MockHelper.h"

extern template struct trompeloeil::reporter<trompeloeil::specialized>;

namespace MemoryTests
{
    /*typedef Memory::MemoryBlock<Memory::FreeListAllocator> MockMemory;

    constexpr uint32_t fooConst = 0xF00BACEC;

    template <typename TMock>
    class IFooMock
    {
    public:
        virtual ~IFooMock() { test = 0;  } // Die();
        IFooMock() : Handle(), test(fooConst) {}
        explicit IFooMock(unsigned t) : Handle(), test(t) {}

        IFooMock(IFooMock&& other) noexcept : Handle(std::move(other.Handle)), test(other.test) { }
        IFooMock& operator=(IFooMock&& other) noexcept
        {
            Handle = std::move(other.Handle);
            test   = other.test;

            other.test = 0;
            other.Handle = Utils::Handle<TMock>();

            return *this;
        }

        IFooMock(const IFooMock&) = delete;
        IFooMock& operator=(const IFooMock&) = delete;

        Utils::Handle<TMock> Handle;
        unsigned test;

        uint32_t Quack()
        { return test; }
    };

    class FooMock : public trompeloeil::deathwatched<IFooMock<FooMock>>
    {
    public:
        explicit FooMock(unsigned t = fooConst)
            : trompeloeil::deathwatched<IFooMock<FooMock>>(t)
        { }
    };

    class FooTrait
    {
    public:
        using object_t = FooMock;
        using handle_t = Utils::Handle<FooMock>;

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

        inline static void setHandle(object_t& obj, uint16_t liveId, uint32_t index) noexcept
        {
            obj.Handle.liveId = liveId;
            obj.Handle.index = index;
        }

        inline static handle_t& getHandle(object_t& obj) noexcept
        {
            return obj.Handle;
        }
    };

    TEST_CASE("ContainerTest")
    {
        MockMemory* memoryBlock;
        unsigned memorySize = 4096 * 2;
        unsigned count = 32;
        void* memory;

        memory = malloc(memorySize);
        memoryBlock = new MockMemory(*new Memory::FreeListAllocator(memory, memorySize), "ContainerBlock");

        YAGE_DISPOSE
        {
            free(memory);
        };

        SECTION("CanCreateContainer")
        {
            auto* container = new Utils::Container<FooTrait>(*memoryBlock, count);

            REQUIRE(container != nullptr);

            delete container;
        }

        SECTION("CanCreateObject")
        {
            Utils::Container<FooTrait> container(*memoryBlock, count);
            FooTrait::handle_t handle = container.emplace(13);

            auto& obj = container.get(handle);

            REQUIRE(handle != FooTrait::handle_t::invalid());
            REQUIRE(handle.key != 0);
            REQUIRE(handle.liveId != 0);
            // ToDo : Implement typeId
            // REQUIRE(handle.typeId == TypeInfo<FooMock>::id());
            REQUIRE(container[handle].test == 13);
            REQUIRE(container.contains(handle));

            REQUIRE_DESTRUCTION(obj);

            container.clear();
        }

        SECTION("CanRemoveObject")
        {
            Utils::Container<FooTrait> container(*memoryBlock, count);
            auto handle = container.emplace();

            REQUIRE_DESTRUCTION(container.get(handle));

            container.erase(handle);

            REQUIRE(!container.contains(handle));
        }

        SECTION("CanReuseObject")
        {
            Utils::Container<FooTrait> container(*memoryBlock, count);

            auto oldHandle = container.emplace();

            REQUIRE_DESTRUCTION(container.get(oldHandle));

            container.erase(oldHandle);
            auto newHandle = container.emplace();

            REQUIRE(container.contains(newHandle));
            REQUIRE(!container.contains(oldHandle));
            REQUIRE(oldHandle.key != newHandle.key);

            REQUIRE_DESTRUCTION(container.get(newHandle));

            container.clear();
        }

        SECTION("CanFreeContainer")
        {
            Utils::Container<FooTrait>* container = new Utils::Container<FooTrait>(*memoryBlock, count);
            auto handle = container->emplace();
            auto& obj = container->get(handle);

            REQUIRE_DESTRUCTION(obj);

            delete container;
        }

        SECTION("CanCreateManyItemsFromContainer")
        {
            Utils::Container<FooTrait>* container = new Utils::Container<FooTrait>(*memoryBlock, count);

            for (int i = 0; i < count; i++)
                container->emplace();

            REQUIRE(container->size() == count);

            TDestructionReqVec vec;
            for (int i = 0; i < count; i++)
                vec.emplace_back(NAMED_REQUIRE_DESTRUCTION(container->at(i)));

            container->clear();
        }

        SECTION("CanRemoveItemFromMiddleOfContainer")
        {
            const int elementCount = 5;
            const int elementToDelete = 2;
            Utils::Container<FooTrait>* container = new Utils::Container<FooTrait>(*memoryBlock, count);

            std::vector<FooTrait::handle_t> handles;

            for (auto i = 0; i < elementCount; i++)
                handles.push_back(container->emplace());

            FooMock& deletedItem = container->get(handles[elementToDelete]);

            REQUIRE(elementCount == container->size());
            REQUIRE_DESTRUCTION(deletedItem);

            container->erase(handles[elementToDelete]);

            for (auto i = 0; i < elementCount; i++)
            {
                if (i != elementToDelete)
                {
                    FooMock& element = container->get(handles[i]);
                    REQUIRE(fooConst == element.test);
                    uint32_t number = element.Quack();
                    REQUIRE(fooConst == number);
                }
            }

            REQUIRE(elementCount - 1 == container->size());

            TDestructionReqVec vec;
            for (int i = 0; i < container->size(); i++)
                vec.emplace_back(NAMED_REQUIRE_DESTRUCTION(container->at(i)));

            container->clear();
        }
    }*/
}
