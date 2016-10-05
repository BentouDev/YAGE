//
// Created by bentoo on 9/30/16.
//

#ifndef GAME_RESOURCEBUNDLE_H
#define GAME_RESOURCEBUNDLE_H

#include <Utils/include/utils/MemoryBlock.h>
#include <Utils/include/utils/FreeListAllocator.h>
#include <Utils/include/utils/TypeInfo.h>

namespace Core
{
	typedef Memory::MemoryBlock<Memory::FreeListAllocator> ResourceMemoryBlock;

	class ResourceBundle
	{
		ResourceMemoryBlock& memoryBlock;

	public:
		ResourceBundle(ResourceMemoryBlock& memory);

		void RegisterResourceType(type_t typeId)
		{

		}

		bool HasResourceType(type_t typeId);
	};
}

#endif //GAME_RESOURCEBUNDLE_H
