//
// Created by bentoo on 24.11.16.
//

#ifndef GAME_ISYSTEM_H
#define GAME_ISYSTEM_H

#include <cstddef>
#include <atomic>
#include <Utils/TypeInfo.h>
#include <Utils/MemoryBlock.h>
#include <Utils/List.h>

#include "Entity.h"

namespace Logic
{
	class ISystem
	{
		static std::atomic<std::size_t> _lastTypeId;

		const char*	_name;
		std::size_t	_systemId;
		type_t		_type;

	protected:
		explicit ISystem(Memory::IMemoryBlock& memory, const char* name, type_t type, std::size_t systemId)
			: _name(name), _systemId(systemId), _type(type), _entities(memory)
		{ }

		template <typename T>
		std::size_t GetSystemId()
		{
			static_assert(std::is_base_of<ISystem, T>::value, "T Must derive from System!");
			static std::size_t id = _lastTypeId++;
			return id;
		}

		Utils::List<Entity::handle_t> _entities;

	public:
		const char* name() const
		{ return _name; }

		std::size_t systemId() const
		{ return _systemId; }

		type_t type() const
		{ return _type; }

		inline const Utils::List<Entity::handle_t>& getEntities() const
		{ return _entities; }

		inline Utils::List<Entity::handle_t>& getEntities()
		{ return _entities; }
	};
}

#endif //GAME_ISYSTEM_H
