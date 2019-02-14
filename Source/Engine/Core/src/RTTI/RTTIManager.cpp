#include <Core/RTTI/RTTIManager.h>
#include <Core/Engine.h>
#include <Utils/MemoryBlock.h>

namespace RTTI
{
	Manager::Manager(Core::Engine& engine, Memory::IMemoryBlock& memory)
		: IManager(engine, memory)
	{

	}
}