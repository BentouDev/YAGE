//
// Created by bentoo on 10/4/16.
//

#ifndef GAME_DICTIONARY_H
#define GAME_DICTIONARY_H

#include "MemoryBlock.h"
#include "List.h"

namespace Utils
{
	template <typename T>
	class Dictionary
	{
		Memory::MemoryBlockBase& _memory;

		struct Node
		{
			uint32_t next;
			uint32_t previous;
			T Value;
		};

		List<Node> Values;
		List<uint32_t> KeyHashes;

	public:
		inline explicit Dictionary(Memory::MemoryBlockBase& memory)
			: _memory(memory), Values(_memory), KeyHashes(memory)
		{

		}

		void add()
		{

		}
	};
}

#endif //GAME_DICTIONARY_H
