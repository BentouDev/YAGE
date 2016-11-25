//
// Created by bentoo on 23.11.16.
//

#include "Core/Logic/World.h"
#include "Core/Logic/EntityManager.h"

namespace Logic
{
	World::World(Memory::IMemoryBlock& memory)
		: _memory(memory), _entityManager(nullptr)
	{
		_entityManager = YAGE_CREATE_NEW(_memory, EntityManager)(_memory);
	}

	World::~World()
	{
		Memory::Delete(_memory, _entityManager);
	}

	void World::update(const Core::GameTime& time)
	{
		// iterate over each system
	}

	bool World::containsEntity(entity_handle_t handle) const
	{
		return _entityManager->contains(handle);
	}

	World::entity_handle_t World::createEntity()
	{
		return _entityManager->createNew();
	}

	void World::removeEntity(entity_handle_t handle)
	{
		assert(containsEntity(handle) && "Cannot remove Entity with invalid handle!");
		_entityManager->remove(handle);
	}

	Entity& World::getEntity(entity_handle_t handle) const
	{
		assert(containsEntity(handle) && "Cannot get Entity with invalid handle!");
		_entityManager->getEntity(handle);
	}

	Entity* World::tryGetEntity(entity_handle_t handle) const
	{
		Entity* result = nullptr;

		if(_entityManager->contains(handle))
			result = &_entityManager->getEntity(handle);

		return result;
	}

	bool World::hasComponent(Entity& entity, comp_id_t bit) const
	{
		return entity.componentBits.test(bit);
	}

	Utils::RawHandle World::getComponent(Entity& entity, comp_id_t bit) const
	{
		return _entityManager->getComponentHandle(entity.Handle, bit);
	}

	void World::addComponent(Entity& entity, comp_id_t bit, Utils::RawHandle handle)
	{
		entity.componentBits.set(bit, true);
		_entityManager->setComponentHandle(entity.Handle, bit, handle);
	}

	void World::removeComponent(Entity& entity, comp_id_t bit)
	{
		entity.componentBits.set(bit, false);
		_entityManager->setComponentHandle(entity.Handle, bit, Utils::RawHandle::invalid());
	}
}
