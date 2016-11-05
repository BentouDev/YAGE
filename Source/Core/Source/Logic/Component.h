//
// Created by bentoo on 10/14/16.
//

#ifndef GAME_COMPONENT_H
#define GAME_COMPONENT_H

#include <Utils/String.h>
#include <Utils/Index.h>
#include <Utils/TypeInfo.h>
#include <Utils/Handle.h>

#define DECL_COMP(name) class name : public Logic::Component<name>

namespace Logic
{
	template <typename Comp>
	class Component
	{
	public:
		using handle_t = Utils::Handle<Comp>;

		handle_t Handle;

	protected:
		explicit Component() : Handle() { }
		Component(Component&& other) : Handle() { }

	public:
		explicit Component(const Component&) = delete;
		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) = delete;

		virtual ~Component() noexcept { }
	};
}

#endif //GAME_COMPONENT_H
