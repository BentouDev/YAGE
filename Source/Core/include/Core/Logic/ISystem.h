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

namespace Core
{
	struct GameTime;
}

namespace Logic
{
	class Entity;

	using system_id_t = std::uint8_t;

	class ISystem
	{
	public:
		friend class World;
		using entity_handle_t = Utils::Handle<Entity>;

	private:
		static std::atomic<system_id_t> _lastTypeId;

		std::bitset<32>	componentBits;
		const char*		_name;
		system_id_t		_systemId;
		type_t			_type;

	protected:
		explicit ISystem(Memory::IMemoryBlock& memory, const char* name, type_t type,
						 std::size_t systemId, std::bitset<32> bits);

		template <typename T>
		system_id_t GetSystemId();

		Utils::List<Entity::handle_t> _entities;

	public:
		inline const char* name() const
		{ return _name; }

		inline std::size_t systemId() const
		{ return _systemId; }

		inline type_t type() const
		{ return _type; }

		inline const Utils::List<Entity::handle_t>& getEntities() const
		{ return _entities; }

		inline Utils::List<Entity::handle_t>& getEntities()
		{ return _entities; }

		virtual void update(const Core::GameTime&) = 0;
		virtual void addEntity(entity_handle_t) = 0;
	};

	template <typename T>
	system_id_t ISystem::GetSystemId()
	{
		static_assert(std::is_base_of<ISystem, T>::value, "T Must derive from System!");
		static system_id_t id = _lastTypeId++;
		return id;
	}
}

#endif //GAME_ISYSTEM_H
