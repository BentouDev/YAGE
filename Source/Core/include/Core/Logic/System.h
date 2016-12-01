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
	public:
		using component_list_t = Utils::TypeIndexList<Components...>;

		static std::bitset<32> BuildBitset()
		{
			std::uint8_t componentBits[] = { IComponent::GetComponentId<Components>()... };
			std::bitset<32> bitset;

			for(auto i = 0; i < sizeof...(Components); i++)
			{
				bitset.set(componentBits[i]);
			}

			return bitset;
		}
	};

	template <typename Sys, typename Reqs>
	class System : public ISystem
	{
	public:
		using requirements = Reqs;

		virtual ~System()
		{ }

	protected:
		explicit System(Memory::IMemoryBlock& memory)
			: ISystem(memory, TypeInfo<Sys>::cName(), TypeInfo<Sys>::id(), ISystem::GetSystemId<Sys>(),
					  Reqs::BuildBitset())
		{ }
	};
}

#endif //GAME_SYSTEM_H
