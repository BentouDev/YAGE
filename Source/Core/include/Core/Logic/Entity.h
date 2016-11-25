//
// Created by bentoo on 24.11.16.
//

#ifndef GAME_ENTITY_H_H
#define GAME_ENTITY_H_H

#include <bitset>
#include <Utils/DefaultTrait.h>

#include "World.h"

namespace Logic
{
	class World;

	class Scene;

	class Entity
	{
		friend class World;

		// TODO: expose component count to compile time setting
		World*			_world;
		std::bitset<32>	componentBits;

	public:
		using handle_t	= Utils::Handle<Entity>;
		using trait_t	= Utils::DefaultTrait<Entity>;

		handle_t Handle;

		explicit Entity();
		virtual ~Entity();

		Entity(Entity&&);

		Entity(const Entity&) = delete;
		Entity& operator=(Entity&&) = delete;
		Entity& operator=(const Entity&) = delete;

		inline World& getWorld() const
		{
			assert(_world != nullptr && "World in Entity cannot be nullptr!");
			return *_world;
		}

		template <typename T>
		bool hasComponent() const
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T must derive from IComponent!");
			return componentBits.test(IComponent::GetComponentId<T>());
		}

		template <typename T>
		T& addComponent()
		{
			return getWorld().addComponent<T>(*this);
		}

		template <typename T>
		void removeComponent()
		{
			return getWorld().removeComponent<T>(*this);
		}

		template <typename T>
		T& getComponent()
		{
			return getWorld().getComponent<T>(*this);
		}

		template <typename T>
		T* tryGetComponent()
		{
			return getWorld().tryGetComponent<T>(*this);
		}
	};
}

#endif //GAME_ENTITY_H_H
