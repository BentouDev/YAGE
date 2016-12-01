//
// Created by bentoo on 29.11.16.
//

#ifndef GAME_WORLDTESTS_H
#define GAME_WORLDTESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Core/Logic/World.h>
#include <Utils/FreeListAllocator.h>
#include <Utils/MemorySizes.h>
#include <Core/Logic/System.h>
#include <Core/Logger.h>

using MemoryBlock = Memory::MemoryBlock
<
	Memory::FreeListAllocator,
	Memory::NoMemoryBoundChecker,
	Memory::NoMemoryTracker
>;

namespace WorldTests
{
	struct MockCallback
	{
		MOCK_METHOD0(Call, void());
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
			if(dieCallback != nullptr)
				dieCallback->Call();
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
			if(dieCallback != nullptr)
				dieCallback->Call();
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

	class WorldTest : public ::testing::Test
	{
	protected:
		static const std::size_t memorySize = Memory::MB(10);

		void* masterMemory = nullptr;
		Memory::FreeListAllocator* allocator = nullptr;

	public:
		void SetUp()
		{
			masterMemory	= malloc(memorySize);
			allocator		= new Memory::FreeListAllocator(masterMemory, memorySize);
			Core::Logger::get()->set_level(spdlog::level::critical);
		}

		void TearDown()
		{
			delete allocator;
			free(masterMemory);
		}
	};

	TEST_F(WorldTest, CanCreateWorld)
	{
		MemoryBlock memory(*allocator, "CanCreateWorld");
		Logic::World* world = new Logic::World(memory);

		EXPECT_NE(world, nullptr);

		delete world;
	}

	TEST_F(WorldTest, CanCreateEntity)
	{
		MemoryBlock memory(*allocator, "CanCreateEntity");
		Logic::World world(memory);

		auto handle = world.createEntity(nullptr);

		EXPECT_NE(Utils::Handle<Logic::Entity>::invalid(), handle);
		EXPECT_TRUE(world.isAlive(handle));
	}

	TEST_F(WorldTest, CanRemoveEntity)
	{
		MemoryBlock memory(*allocator, "CanRemoveEntity");
		Logic::World world(memory);

		auto handle = world.createEntity(nullptr);
		world.removeEntity(handle);

		EXPECT_FALSE(world.isAlive(handle));
		EXPECT_TRUE(world.isAliveInCurrentFrame(handle));

		world.refresh();

		EXPECT_FALSE(world.isAlive(handle));
		EXPECT_FALSE(world.isAliveInCurrentFrame(handle));
	}

	TEST_F(WorldTest, CanCreateAndRegisterSystem)
	{
		MemoryBlock memory(*allocator, "CanCreateAndRegisterSystem");
		Logic::World world(memory);

		EXPECT_FALSE(world.hasSystem<MockSystem>());

		world.createAndRegisterSystem<MockSystem>(memory);

		EXPECT_TRUE(world.hasSystem<MockSystem>());
	}

	TEST_F(WorldTest, IsSystemProperlyDestructed)
	{
		MockCallback onDie;

		EXPECT_CALL(onDie, Call());
		{
			MemoryBlock memory(*allocator, "IsSystemProperlyDestructed");
			Logic::World world(memory);
			world.createAndRegisterSystem<MockSystem>(memory, &onDie);
		}
	}

	TEST_F(WorldTest, CanCreateComponent)
	{
		MemoryBlock memory(*allocator, "CanCreateComponent");
		Logic::World world(memory);

		auto handle = world.createEntity(nullptr);
		world.createAndRegisterSystem<MockSystem>(memory);
		world.addComponent<MockComponent>(world.getEntity(handle));

		EXPECT_TRUE(world.hasComponent<MockComponent>(handle));
	}

	TEST_F(WorldTest, CanRemoveComponent)
	{
		MemoryBlock memory(*allocator, "CanRemoveComponent");
		Logic::World world(memory);

		auto handle = world.createEntity(nullptr);
		world.createAndRegisterSystem<MockSystem>(memory);
		world.addComponent<MockComponent>(world.getEntity(handle));
		world.removeComponent<MockComponent>(world.getEntity(handle));

		EXPECT_FALSE(world.hasComponent<MockComponent>(handle));
	}

	TEST_F(WorldTest, AreComponentsProperlyDestructed)
	{
		MockCallback onDie;

		EXPECT_CALL(onDie, Call());
		{
			MemoryBlock memory(*allocator, "AreComponentsProperlyDestructed");
			Logic::World world(memory);

			auto handle = world.createEntity(nullptr);
			world.createAndRegisterSystem<MockSystem>(memory);
			world.addComponent<MockComponent>(world.getEntity(handle), &onDie);
		}
	}

	TEST_F(WorldTest, AreMatchingEntitiesAddedToSystem)
	{
		MockCallback onAddEntity;

		EXPECT_CALL(onAddEntity, Call());

		MemoryBlock memory(*allocator, "AreMatchingEntitiesAddedToSystem");
		Logic::World world(memory);

		auto handle = world.createEntity(nullptr);
		world.createAndRegisterSystem<MockSystem>(memory, nullptr, &onAddEntity);
		world.addComponent<MockComponent>(world.getEntity(handle));
		world.refresh();
	}

	TEST_F(WorldTest, AreUnmatchedEntititesRemovedFromSystem)
	{
		MockCallback onRemoveEntity;

		EXPECT_CALL(onRemoveEntity, Call());

		MemoryBlock memory(*allocator, "AreUnmatchedEntititesRemovedFromSystem");
		Logic::World world(memory);

		auto handle = world.createEntity(nullptr);
		world.createAndRegisterSystem<MockSystem>(memory, nullptr, nullptr, &onRemoveEntity);
		world.addComponent<MockComponent>(world.getEntity(handle));
		world.refresh();

		world.removeComponent<MockComponent>(world.getEntity(handle));
		world.refresh();
	}

	TEST_F(WorldTest, AreRemovedEntititesRemovedFromSystem)
	{
		MockCallback onRemoveEntity;

		EXPECT_CALL(onRemoveEntity, Call());

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

#endif //GAME_WORLDTESTS_H
