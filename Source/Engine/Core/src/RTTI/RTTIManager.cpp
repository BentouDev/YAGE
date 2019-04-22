#include <Core/RTTI/RTTIManager.h>
#include <Core/Engine.h>
#include <Utils/MemoryBlock.h>

//template <typename TLayer>
//int LayerTypeGuard<TLayer>::_counter{ 0 };
//template<> RTTI::IntegralLayer* LayerHelper<RTTI::IntegralLayer>::_instance{ nullptr };

namespace RTTI
{
	Manager::Manager(Core::Engine& engine, Memory::IMemoryBlock& memory)
		: IManager(engine, memory)
	{

	}
}