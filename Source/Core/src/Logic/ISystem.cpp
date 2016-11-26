//
// Created by bentoo on 26.11.16.
//

#include "Core/Logic/ISystem.h"
#include "Core/Logger.h"

namespace Logic
{
	std::atomic<type_t> ISystem::_lastTypeId { 0 };

	ISystem::ISystem(Memory::IMemoryBlock& memory, const char* name, type_t type,
					 std::size_t systemId, std::bitset<32> bits)
		: componentBits(bits), _name(name), _systemId(systemId), _type(type), _entities(memory)
	{
		Core::Logger::get()->info("Created system '{}' with memory '{}'", this->name(), memory.getName());
	}
}