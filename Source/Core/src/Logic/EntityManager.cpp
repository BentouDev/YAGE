//
// Created by bentoo on 25.11.16.
//

#include "Core/Logic/EntityManager.h"

namespace Logic
{
	EntityManager::EntityManager(Memory::IMemoryBlock &memory)
		: _memory(memory), _entityContainer(_memory)
	{

	}

	EntityManager::~EntityManager()
	{

	}

	EntityManager::handle_t EntityManager::createNew()
	{
		_entityContainer.create();
	}

	Utils::RawHandle EntityManager::getComponentHandle(handle_t handle, comp_id_t bit) const
	{
		return _entityContainer.get(handle, bit);
	}

	void EntityManager::setComponentHandle(handle_t handle, comp_id_t bit, Utils::RawHandle comp)
	{
		_entityContainer.set(handle, bit, comp);
	}

	bool EntityManager::contains(handle_t handle) const
	{
		return _entityContainer.contains(handle);
	}

	void EntityManager::remove(handle_t handle)
	{
		_entityContainer.remove(handle);
	}

	Entity& EntityManager::getEntity(handle_t handle) const
	{
		return _entityContainer.get(handle);
	}

	Entity* EntityManager::tryGetEntity(handle_t handle) const
	{
		Entity* result = nullptr;

		if(_entityContainer.contains(handle))
			result = &_entityContainer.get(handle);

		return result;
	}
}
