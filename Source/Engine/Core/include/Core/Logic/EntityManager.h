//
// Created by bentoo on 24.11.16.
//

#ifndef GAME_ENTITYMANAGER_H
#define GAME_ENTITYMANAGER_H

#include <Utils/Handle.h>
#include <Utils/HandleContainer.h>
#include "Entity.h"

namespace Logic
{
	class World;
	class Scene;

	class EntityManager
	{
	public:
		using handle_t	= Entity::handle_t;
		using trait_t	= Entity::trait_t;
		using storage_t = Utils::HandleContainer<trait_t, Utils::RawHandle, 32>;

	protected:
		Memory::IMemoryBlock&	_memory;
		storage_t				_entityContainer;

	public:
		explicit EntityManager(Memory::IMemoryBlock& memory);
		virtual ~EntityManager() noexcept;

		auto createNew(World* world, Scene* scene) -> handle_t;

		void remove(handle_t handle);

		bool contains(handle_t handle) const;

		auto getComponentHandle(handle_t handle, comp_id_t bit) const -> Utils::RawHandle;
		void setComponentHandle(handle_t handle, comp_id_t bit, Utils::RawHandle comp);

		auto getEntity(handle_t handle) const -> Entity&;

		auto tryGetEntity(handle_t) const -> Entity*;
	};
}

#endif //GAME_ENTITYMANAGER_H
