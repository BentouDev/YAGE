//
// Created by bentoo on 23.11.16.
//

#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <bitset>
#include <Utils/Container.h>
#include <Utils/Assert.h>

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
		struct SystemInfo;

		Memory::IMemoryBlock&	_memory;
		EntityManager*			_entityManager;
		Utils::IContainer*		_componentContainers[32];

		Utils::List<SystemInfo>			_registeredSystems;
		Utils::List<entity_handle_t>	_dirtyEntities;
		Utils::List<entity_handle_t>	_removedEntities;

		auto getComponent(entity_handle_t, comp_id_t) const -> Utils::RawHandle;
		auto getComponent(Entity&, comp_id_t) const -> Utils::RawHandle;


		bool hasComponent(Entity&, comp_id_t) const;
		void addComponent(Entity&, comp_id_t, Utils::RawHandle);
		void removeComponent(Entity&, comp_id_t);

		void setAsDirty(Entity&);
		void setAsRemoved(Entity&);

		bool matchComponentSignature(std::bitset<32>, std::bitset<32>);

		template <typename T>
		class ComponentContainerFunctor
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");

			using trait_t = typename T::trait_t;

			World* world;

		public:
			explicit ComponentContainerFunctor(World* world)
				: world(world)
			{ }

			void operator()()
			{
				Utils::IContainer** containerPtr = &world->_componentContainers[IComponent::GetComponentId<T>()];
				if((*containerPtr) == nullptr)
				{
					(*containerPtr) = YAGE_CREATE_NEW(world->_memory, Utils::Container<trait_t>)(world->_memory);
				}
			}
		};

		template <typename ComponentList>
		void createComponentContainers()
		{
			ComponentList::template foreach<ComponentContainerFunctor>(this);
		}

		template <typename T>
		Utils::Container<typename T::trait_t>& getComponentContainer() const
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");
			using trait_t = typename T::trait_t;

			Utils::IContainer* containerPtr = _componentContainers[IComponent::GetComponentId<T>()];

			return *static_cast<Utils::Container<trait_t>*>(containerPtr);
		}

		// TODO : better system handling, have a TypeIndexList, use it!
		struct SystemInfo
		{
			SystemInfo(ISystem* ptr, type_t id)
				: instance(ptr), type(id)
			{ }

			ISystem*	instance;
			type_t		type;
		};

	public:
		explicit World(Memory::IMemoryBlock& memory);
		virtual ~World();

		void refresh();

		void update(const Core::GameTime& time, bool andRefresh = true);

		bool isAlive(entity_handle_t) const;

		bool isAliveInCurrentFrame(entity_handle_t) const;

		auto createEntity(Scene* scene) -> entity_handle_t;

		void removeEntity(entity_handle_t);

		auto getEntity(entity_handle_t) const -> Entity&;

		auto tryGetEntity(entity_handle_t) const -> Entity*;

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
			static_assert(std::is_base_of<ISystem, T>::value, "TSystem must derive from ISystem!");

			for(SystemInfo& info : _registeredSystems)
			{
				if(info.type == TypeInfo<T>::id())
					return true;
			}

			return false;
		}

		template <typename T, typename ... Args>
		void createAndRegisterSystem(Args&& ... args)
		{
			static_assert(std::is_base_of<ISystem, T>::value, "T must derive from ISystem!");

			YAGE_ASSERT(!hasSystem<T>(), "World : Cannot register '%s' system twice!", TypeInfo<T>::cName());

			createComponentContainers<typename T::requirements::component_list_t>();

			T* instance = YAGE_CREATE_NEW(_memory, T)(std::forward<Args>(args)...);
			_registeredSystems.emplace(instance, TypeInfo<T>::id());
		};
	};
}

#endif //GAME_WORLD_H
