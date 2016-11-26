//
// Created by bentoo on 23.11.16.
//

#include "Core/Logic/World.h"
#include "Core/Logic/EntityManager.h"
#include "Core/Logic/ISystem.h"

namespace Logic
{
	World::World(Memory::IMemoryBlock& memory)
		: _memory(memory), _entityManager(nullptr),
		  _registeredSystems(_memory), _dirtyEntities(_memory)
	{
		_entityManager = YAGE_CREATE_NEW(_memory, EntityManager)(_memory);
	}

	World::~World()
	{
		Memory::Delete(_memory, _entityManager);
	}

	void World::update(const Core::GameTime& time)
	{
		for(auto handle : _dirtyEntities)
		{
			Entity& entity = getEntity(handle);
			entity._status.dirty = false;

			for (SystemInfo& info : _registeredSystems)
			{
				if(componentSignatureMatches(entity.componentBits, info.instance->componentBits))
				{
					info.instance->addEntity(entity.Handle);
				}
			}
		}

		_dirtyEntities.clear();

		// iterate over each system
		for(SystemInfo& info : _registeredSystems)
		{
			info.instance->update(time);
		}
	}

	bool World::componentSignatureMatches(std::bitset<32> entity, std::bitset<32> system)
	{
		return ((entity & system) == system);
	}

	void World::setDirty(Entity& entity)
	{
		if(!entity._status.dirty)
		{
			entity._status.dirty = true;
			_dirtyEntities.add(entity.Handle);
		}
	}

	bool World::containsEntity(entity_handle_t handle) const
	{
		return _entityManager->contains(handle);
	}

	auto World::createEntity(Scene* scene) -> World::entity_handle_t
	{
		return _entityManager->createNew(this, scene);
	}

	void World::removeEntity(entity_handle_t handle)
	{
		assert(containsEntity(handle) && "Cannot remove Entity with invalid handle!");
		_entityManager->remove(handle);
	}

	auto World::getEntity(entity_handle_t handle) const -> Entity&
	{
		assert(containsEntity(handle) && "Cannot get Entity with invalid handle!");
		return _entityManager->getEntity(handle);
	}

	auto World::tryGetEntity(entity_handle_t handle) const -> Entity*
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

	auto World::getComponent(entity_handle_t handle, comp_id_t bit) const -> Utils::RawHandle
	{
		return _entityManager->getComponentHandle(handle, bit);
	}

	auto World::getComponent(Entity& entity, comp_id_t bit) const -> Utils::RawHandle
	{
		return getComponent(entity.Handle, bit);
	}

	void World::addComponent(Entity& entity, comp_id_t bit, Utils::RawHandle handle)
	{
		entity.componentBits.set(bit, true);
		_entityManager->setComponentHandle(entity.Handle, bit, handle);

		// TODO : Notify systems about that, or put entity to list and update later
		setDirty(entity);
	}

	void World::removeComponent(Entity& entity, comp_id_t bit)
	{
		entity.componentBits.set(bit, false);
		_entityManager->setComponentHandle(entity.Handle, bit, Utils::RawHandle::invalid());

		// TODO : Notify systems about that, or put entity to list and update later
		setDirty(entity);
	}
}
