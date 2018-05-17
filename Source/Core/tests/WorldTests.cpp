//
// Created by bentoo on 29.11.16.
//

#ifndef GAME_WORLDTESTS_H
#define GAME_WORLDTESTS_H

#include <catch.hpp>
#include <trompeloeil.hpp>

#include <Core/Logic/World.h>
#include <Utils/FreeListAllocator.h>
#include <Utils/MemorySizes.h>
#include <Utils/ScopeGuard.h>
#include <Core/Logic/System.h>
#include <Core/Logger.h>

using MemoryBlock = Memory::MemoryBlock
<
    Memory::FreeListAllocator,
    Memory::NoMemoryBoundChecker,
    Memory::NoMemoryTracker
>;

extern template struct trompeloeil::reporter<trompeloeil::specialized>;

namespace WorldTests
{
    struct MockCallback
    {
        MAKE_MOCK0(Call, void());
    };

    class MockComponent : public Logic::Component<MockComponent>
    {
        MockCallback* dieCallback;

    public:
        explicit MockComponent(MockCallback* callback = nullptr)
            : dieCallback(callback)
        { };

        MockComponent(MockComponent&& other)
            : dieCallback(other.dieCallback)
        {
            other.dieCallback = nullptr;
        }

        virtual ~MockComponent()
        {
            //if(dieCallback != nullptr)
            //	dieCallback->Call();
        }
    };

    class MockSystem : public Logic::System<MockSystem, Logic::Requires<MockComponent>>
    {
        MockCallback* dieCallback;
        MockCallback* addCallback;
        MockCallback* removeCallback;

    public:
        explicit MockSystem(Memory::IMemoryBlock &memory,
                            MockCallback* destruct = nullptr,
                            MockCallback* addEntity = nullptr,
                            MockCallback* removeEntity = nullptr)
            : System(memory), dieCallback(destruct),
              addCallback(addEntity), removeCallback(removeEntity)
        { }

        virtual ~MockSystem()
        {
            //if(dieCallback != nullptr)
            //	dieCallback->Call();
        }

        void update(const Core::GameTime& time) override
        { }

        void addEntity(Utils::Handle<Logic::Entity> handle) override
        {
            if(addCallback != nullptr)
                addCallback->Call();

            Logic::ISystem::addEntity(handle);
        }

        void removeEntity(Utils::Handle<Logic::Entity> handle) override
        {
            if(removeCallback != nullptr)
                removeCallback->Call();

            Logic::ISystem::removeEntity(handle);
        }
    };

    TEST_CASE("WorldTest")
    {
        static const std::size_t memorySize = Memory::MB(10);

        void* masterMemory = nullptr;
        Memory::FreeListAllocator* allocator = nullptr;

        masterMemory = malloc(memorySize);
        allocator = new Memory::FreeListAllocator(masterMemory, memorySize);
        Core::Logger::setLogLevel(Core::LogLevel::critical);

        YAGE_DISPOSE
        {
            delete allocator;
            free(masterMemory);
        };

        SECTION("CanCreateWorld")
        {
            MemoryBlock memory(*allocator, "CanCreateWorld");
            Logic::World* world = new Logic::World(memory);

            REQUIRE(world != nullptr);

            delete world;
        }

        SECTION("CanCreateEntity")
        {
            MemoryBlock memory(*allocator, "CanCreateEntity");
            Logic::World world(memory);

            auto handle = world.createEntity(nullptr);

            REQUIRE(Utils::Handle<Logic::Entity>::invalid() != handle);
            REQUIRE(world.isAlive(handle));
        }

        SECTION("CanRemoveEntity")
        {
            MemoryBlock memory(*allocator, "CanRemoveEntity");
            Logic::World world(memory);

            auto handle = world.createEntity(nullptr);
            world.removeEntity(handle);

            REQUIRE(!world.isAlive(handle));
            REQUIRE(world.isAliveInCurrentFrame(handle));

            world.refresh();

            REQUIRE(!world.isAlive(handle));
            REQUIRE(!world.isAliveInCurrentFrame(handle));
        }

        SECTION("CanCreateAndRegisterSystem")
        {
            MemoryBlock memory(*allocator, "CanCreateAndRegisterSystem");
            Logic::World world(memory);

            REQUIRE(!world.hasSystem<MockSystem>());

            world.createAndRegisterSystem<MockSystem>(memory);

            REQUIRE(world.hasSystem<MockSystem>());
        }

        SECTION("IsSystemProperlyDestructed")
        {
            MockCallback onDie;

            // REQUIRE_CALL(onDie, Call()).TIMES(1);
            {
                MemoryBlock memory(*allocator, "IsSystemProperlyDestructed");
                Logic::World world(memory);
                world.createAndRegisterSystem<MockSystem>(memory, &onDie);
            }
        }

        SECTION("CanCreateComponent")
        {
            MemoryBlock memory(*allocator, "CanCreateComponent");
            Logic::World world(memory);

            auto handle = world.createEntity(nullptr);
            world.createAndRegisterSystem<MockSystem>(memory);
            world.addComponent<MockComponent>(world.getEntity(handle));

            REQUIRE(world.hasComponent<MockComponent>(handle));
        }

        SECTION("CanRemoveComponent")
        {
            MemoryBlock memory(*allocator, "CanRemoveComponent");
            Logic::World world(memory);

            auto handle = world.createEntity(nullptr);
            world.createAndRegisterSystem<MockSystem>(memory);
            world.addComponent<MockComponent>(world.getEntity(handle));
            world.removeComponent<MockComponent>(world.getEntity(handle));

            REQUIRE(!world.hasComponent<MockComponent>(handle));
        }

        SECTION("AreComponentsProperlyDestructed")
        {
            MockCallback onDie;

            //REQUIRE_CALL(onDie, Call()).TIMES(1);
            {
                MemoryBlock memory(*allocator, "AreComponentsProperlyDestructed");
                Logic::World world(memory);

                auto handle = world.createEntity(nullptr);
                world.createAndRegisterSystem<MockSystem>(memory);
                world.addComponent<MockComponent>(world.getEntity(handle), &onDie);
            }
        }

        SECTION("AreMatchingEntitiesAddedToSystem")
        {
            MockCallback onAddEntity;

            REQUIRE_CALL(onAddEntity, Call()).TIMES(1);

            MemoryBlock memory(*allocator, "AreMatchingEntitiesAddedToSystem");
            Logic::World world(memory);

            auto handle = world.createEntity(nullptr);
            world.createAndRegisterSystem<MockSystem>(memory, nullptr, &onAddEntity);
            world.addComponent<MockComponent>(world.getEntity(handle));
            world.refresh();
        }

        SECTION("AreUnmatchedEntititesRemovedFromSystem")
        {
            MockCallback onRemoveEntity;

            REQUIRE_CALL(onRemoveEntity, Call()).TIMES(1);

            MemoryBlock memory(*allocator, "AreUnmatchedEntititesRemovedFromSystem");
            Logic::World world(memory);

            auto handle = world.createEntity(nullptr);
            world.createAndRegisterSystem<MockSystem>(memory, nullptr, nullptr, &onRemoveEntity);
            world.addComponent<MockComponent>(world.getEntity(handle));
            world.refresh();

            world.removeComponent<MockComponent>(world.getEntity(handle));
            world.refresh();
        }

        SECTION("AreRemovedEntititesRemovedFromSystem")
        {
            MockCallback onRemoveEntity;

            REQUIRE_CALL(onRemoveEntity, Call()).TIMES(1);

            MemoryBlock memory(*allocator, "AreRemovedEntititesRemovedFromSystem");
            Logic::World world(memory);

            auto handle = world.createEntity(nullptr);
            world.createAndRegisterSystem<MockSystem>(memory, nullptr, nullptr, &onRemoveEntity);
            world.addComponent<MockComponent>(world.getEntity(handle));
            world.refresh();

            world.removeEntity(handle);
            world.refresh();
        }
    }
}

#endif //GAME_WORLDTESTS_H
