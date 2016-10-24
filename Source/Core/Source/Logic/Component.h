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
		explicit Component() { }
		explicit Component(Component&& other) { }

	public:
		explicit Component(const Component&) = delete;

		virtual ~Component(){ }

		virtual auto swap(Comp& other) noexcept -> void = 0;
		virtual auto cleanUp() noexcept -> void = 0;
	};
}

#endif //GAME_COMPONENT_H
