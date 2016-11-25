//
// Created by bentoo on 23.11.16.
//

#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <Utils/Container.h>

#include "Component.h"

namespace Core
{
	struct GameTime;
}

namespace Gfx
{
	class Renderer;
}

namespace Logic
{
	class ISystem;

	class Entity;

	class EntityManager;

	class World
	{
	public:
		using entity_handle_t = Utils::Handle<Entity>;

		// TODO: expose component count to compile time setting
	protected:
		Memory::IMemoryBlock&	_memory;
		EntityManager*			_entityManager;
		void*					_componentContainers[32];

		bool hasComponent(Entity&, comp_id_t) const;
		auto getComponent(Entity&, comp_id_t) const -> Utils::RawHandle;
		void addComponent(Entity&, comp_id_t, Utils::RawHandle);
		void removeComponent(Entity&, comp_id_t);

	public:
		explicit World(Memory::IMemoryBlock& memory);
		virtual ~World();

		void update(const Core::GameTime& time);

		bool containsEntity(entity_handle_t) const;

		auto createEntity() -> entity_handle_t;

		void removeEntity(entity_handle_t);

		auto getEntity(entity_handle_t) const -> Entity&;

		auto tryGetEntity(entity_handle_t) const -> Entity*;

		template <typename T>
		Utils::Container<typename T::trait_t>& getComponentContainer() const
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");

			void* containerPtr = _componentContainers[IComponent::GetComponentId<T>()];

			return *static_cast<Utils::Container<typename T::trait_t>*>(containerPtr);
		}

		template <typename T>
		bool hasComponent(entity_handle_t h) const
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");
			return hasComponent(getEntity(h), IComponent::GetComponentId<T>());
		}

		template <typename T>
		bool hasComponent(Entity& e) const
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");
			return hasComponent(e, IComponent::GetComponentId<T>());
		}

		template <typename T, typename ... Args>
		T& addComponent(Entity& e, Args&& ... args)
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");

			auto&	container	= getComponentContainer<T>();
			auto	handle		= container.create(std::forward(args)...);

			addComponent(e, IComponent::GetComponentId<T>(), handle);

			return static_cast<T&>(container.get(handle));
		}

		template <typename T>
		void removeComponent(Entity& e)
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");
			removeComponent(e, IComponent::GetComponentId<T>());
		}

		template <typename T>
		T& getComponent(const Entity& e) const
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");

			Utils::RawHandle handle = getComponent(e, IComponent::GetComponentId<T>());

			return getComponentContainer<T>().get(Utils::handle_cast<T::handle_t>(handle));
		}

		template <typename T>
		T* tryGetComponent(Entity& e)
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");

			Utils::RawHandle	handle		= getComponent(e, IComponent::GetComponentId<T>());
			auto&				container	= getComponentContainer<T>();

			if((bool)handle && container.contains(handle))
				return getComponent<T>(e);

			return nullptr;
		}

		template <typename T>
		bool hasSystem()
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");
		}

		template <typename T>
		void registerSystem()
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");
		};
	};
}

#endif //GAME_WORLD_H
