//
// Created by mrjaqbq on 06.03.16.
//

#ifndef VOLKHVY_RESOURCEMANAGER_H
#define VOLKHVY_RESOURCEMANAGER_H

#include "Resource.h"
#include <Container.h>

namespace Core
{
	class Engine;

	template<class Trait>
	class ResourceManager
	{
		using handle_t = typename Trait::handle;
		Utils::Container<Trait> container;

		template <typename... Args>
		auto create(Args... args) -> handle_t
		{
			return container.create(args...);
		}

		auto getContainer() -> Utils::Container<Trait>&
		{
			return container;
		}
	};
}

#endif //VOLKHVY_RESOURCEMANAGER_H
