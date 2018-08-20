//
// Created by bentoo on 10/4/16.
//

#ifndef GAME_DICTIONARY_H
#define GAME_DICTIONARY_H

#include "MemoryBlock.h"
#include "List.h"

namespace Utils
{
	template <typename key_t, typename value_t>
	class Dictionary
	{
	public:
		using hashFunction = uint32_t(*)(key_t);

	private:
		Memory::IMemoryBlock& _memory;

		struct Node
		{
			uint32_t next;
			uint32_t previous;
			value_t Value;
		};

		List<Node> 		_values;
		List<uint32_t> 	_keyHashes;
		hashFunction	_hash;

	public:
		inline explicit Dictionary(Memory::IMemoryBlock& memory, hashFunction hash)
			: _memory(memory), _values(_memory), _keyHashes(memory), _hash(hash)
		{

		}

		void add()
		{

		}
	};
}

#endif //GAME_DICTIONARY_H
