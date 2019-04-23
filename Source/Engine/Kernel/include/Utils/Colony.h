#ifndef YAGE_COLONY_H
#define YAGE_COLONY_H

#include "SafeObjectTrait.h"
#include "ColonyBase.h"

namespace Utils
{
	template <typename TObject>
	class Colony : public ColonyBase<SafeObjectTrait<TObject>>
	{
		static_assert(std::is_base_of<yage::SafeObject, TObject>::value, "T must derive from SafeObject!");

	public:
		explicit Colony(Memory::IMemoryBlock& memory, std::size_t reserved_size = 0)
			: ColonyBase(memory, reserved_size)
		{ }
	};
}

#endif