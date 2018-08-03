//
// Created by bentoo on 10/14/16.
//

#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#include <Utils/Index.h>
#include <Utils/String.h>
#include <Utils/Handle.h>
#include <Utils/TypeInfo.h>
#include <Utils/DefaultTrait.h>

#define DECL_COMP(name) class name : public Logic::Component<name>

namespace Logic
{
	using comp_id_t = std::uint8_t;

	class IComponent
	{
		static std::atomic<comp_id_t> _lastTypeId;

	public:
		template <typename T>
		static comp_id_t GetComponentId()
		{
			static_assert(std::is_base_of<IComponent, T>::value, "T Must derive from System!");
			static comp_id_t id = _lastTypeId++;
			return id;
		}
	};

	template <typename Comp>
	class Component : public IComponent
	{
	public:
		using handle_t	= Utils::Handle<Comp>;
		using trait_t	= Utils::DefaultTrait<Comp>;

		handle_t Handle;

	protected:
		explicit Component() : Handle() { }
		Component(Component&& other) : Handle() { }

	public:
		Component(const Component&) = delete;
		Component& operator=(Component&&) = delete;
		Component& operator=(const Component&) = delete;

		virtual ~Component() noexcept { }
	};
}

#endif //GAME_COMPONENT_H
