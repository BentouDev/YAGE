//
// Created by bentoo on 19.05.18.
//

#include <catch.hpp>
#include <trompeloeil.hpp>
#include <Utils/Handle.h>
#include <Utils/SmartHandle.h>
#include <Utils/FreeListAllocator.h>

namespace SmartHandleTests
{
    /*typedef Memory::MemoryBlock<Memory::FreeListAllocator> MockMemory;

    constexpr uint32_t fooConst = 0xF00BACEC;

    template <typename TMock>
    class IFooMock
    {
    public:
        virtual ~IFooMock() { test = 0; } // Die();
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

    class FooBase : public IFooMock<FooBase>
    {
    public:
        explicit FooBase(unsigned t = fooConst)
            : IFooMock(t)
        { }
    };

    template <typename TFoo = FooMock>
    class FooTrait
    {
    public:
        using object_t = TFoo;
        using handle_t = Utils::Handle<TFoo>;

        inline static void cleanUp(object_t& first)
        { }

        inline static void swap(object_t& first, object_t& second) noexcept
        { }

        inline static void incrementLiveId(Utils::Index<handle_t>& index) noexcept
        { index.handle.liveId++; }

        inline static void setIndex(Utils::Index<handle_t>& index, uint16_t i) noexcept
        { index.handle.index = i; }

        inline static uint16_t getIndex(Utils::Index<handle_t>& index) noexcept
        { return index.handle.index; }

        inline static uint16_t getIndex(handle_t _handle) noexcept
        { return _handle.index; }

        inline static void setHandle(object_t& obj, uint16_t liveId, uint32_t index) noexcept
        { obj.Handle.liveId = liveId; obj.Handle.index = index; }

        inline static handle_t& getHandle(object_t& obj) noexcept
        { return obj.Handle; }
    };

    TEST_CASE("SmartHandleTests")
    {
        MockMemory* memoryBlock;
        unsigned memorySize = 4096 * 2;
        unsigned count = 32;
        void* memory;

        memory = malloc(memorySize);
        memoryBlock = new MockMemory(*new Memory::FreeListAllocator(memory, memorySize), "ContainerBlock");

        SECTION("IsNullByDefault")
        {
            Utils::SmartHandle<FooTrait<FooBase>> handle;

            REQUIRE(!handle);
            REQUIRE(handle.get() == nullptr);
        }

        SECTION("IsNullByDefault")
        {
            Utils::Colony<FooTrait<FooBase>>      colony(*memoryBlock);
            Utils::SmartHandle<FooTrait<FooBase>> handle = Utils::make_handle(&colony);

            REQUIRE(handle);
            REQUIRE(handle.get() != nullptr);
        }
    }*/
}