//
// Created by bentoo on 21.11.16.
//

#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

#include <type_traits>
#include <bitset>
#include <Utils/TypeIndexList.h>
#include <Utils/ForEachType.h>

#include "Component.h"
#include "ISystem.h"

namespace Logic
{
	template <typename ... Components>
	class Requires
	{
		using component_list_t = Utils::TypeIndexList<Components...>;
	};

	template <typename Sys, typename Reqs>
	class System : private ISystem
	{
	protected:
		explicit System(Memory::IMemoryBlock& memory)
			: ISystem(memory, TypeInfo<Sys>::cName(), TypeInfo<Sys>::id(), ISystem::GetSystemId<Sys>())
		{
	//		Core::Logger::get()->info("Created system '{}' with memory '{}'", name(), memory.getName());
		}
	};
}

#endif //GAME_SYSTEM_H
