//
// Created by bentoo on 23.11.16.
//

#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <Utils/Container.h>
#include <bitset>

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

	class Scene;

	class World
	{
	public:
		using entity_handle_t = Utils::Handle<Entity>;

		// TODO: expose component count to compile time setting
	protected:
		Memory::IMemoryBlock&	_memory;
		EntityManager*			_entityManager;
		void*					_componentContainers[32];

		auto getComponent(entity_handle_t, comp_id_t) const -> Utils::RawHandle;
		auto getComponent(Entity&, comp_id_t) const -> Utils::RawHandle;

		bool hasComponent(Entity&, comp_id_t) const;
		void addComponent(Entity&, comp_id_t, Utils::RawHandle);
		void removeComponent(Entity&, comp_id_t);
		void setDirty(Entity&);

		bool componentSignatureMatches(std::bitset<32>, std::bitset<32>);

		// TODO : better system handling, have a TypeIndexList, use it!
		struct SystemInfo
		{
			SystemInfo(ISystem* ptr, type_t id)
				: instance(ptr), type(id)
			{ }

			ISystem*	instance;
			type_t		type;
		};

		Utils::List<SystemInfo>			_registeredSystems;
		Utils::List<entity_handle_t>	_dirtyEntities;

	public:
		explicit World(Memory::IMemoryBlock& memory);
		virtual ~World();

		void update(const Core::GameTime& time);

		bool containsEntity(entity_handle_t) const;

		auto createEntity(Scene* scene) -> entity_handle_t;

		void removeEntity(entity_handle_t);

		auto getEntity(entity_handle_t) const -> Entity&;

		auto tryGetEntity(entity_handle_t) const -> Entity*;

		template <typename T>
		void debugRegisterComponent()
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");
			_componentContainers[IComponent::GetComponentId<T>()] =
					YAGE_CREATE_NEW(_memory, Utils::Container<typename T::trait_t>)(_memory);
		}

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
			auto	handle		= container.create(std::forward<Args>(args)...);

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
		T& getComponent(entity_handle_t e) const
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");

			Utils::RawHandle	handle		= getComponent(e, IComponent::GetComponentId<T>());
			auto&				container	= getComponentContainer<T>();

			return container.get(Utils::handle_cast<typename T::handle_t>(handle));
		}

		template <typename T>
		T& getComponent(Entity& e) const
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");

			Utils::RawHandle	handle		= getComponent(e, IComponent::GetComponentId<T>());
			auto&				container	= getComponentContainer<T>();

			return container.get(Utils::handle_cast<typename T::handle_t>(handle));
		}

		template <typename T>
		T* tryGetComponent(Entity& e)
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");

			Utils::RawHandle		rawhandle	= getComponent(e, IComponent::GetComponentId<T>());
			typename T::handle_t	handle		= Utils::handle_cast<typename T::handle_t>(rawhandle);
			auto&					container	= getComponentContainer<T>();

			if((bool)handle && container.contains(handle))
				return &container.get(handle);

			return nullptr;
		}

		template <typename T>
		bool hasSystem()
		{
			static_assert(std::is_base_of<ISystem, T>::value, "T must derive from ISystem!");
			for(SystemInfo& info : _registeredSystems)
			{
				if(info.type == TypeInfo<T>::id())
					return true;
			}

			return false;
		}

		template <typename T, typename ... Args>
		void registerSystem(Args&& ... args)
		{
			static_assert(std::is_base_of<ISystem, T>::value, "T must derive from ISystem!");
			assert(!hasSystem<T>() && "World : Cannot register same system twice!");

			T* instance = YAGE_CREATE_NEW(_memory, T)(std::forward<Args>(args)...);

			_registeredSystems.emplace(instance, TypeInfo<T>::id());
		};
	};
}

#endif //GAME_WORLD_H
