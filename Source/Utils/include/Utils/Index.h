//
// Created by mrjaqbq on 13.05.16.
//

#ifndef GAME_INDEX_H
#define GAME_INDEX_H

#include <stdint.h>

namespace Utils
{
	struct Nothing
	{
		int _[0];
	};

	static_assert(sizeof(Nothing) == 0, "Nothing must have size 0!");

	template<typename Handle = Nothing>
	struct Index
	{
		Handle handle;
		uint16_t index;
		uint16_t next;
		uint8_t active : 1;
		uint8_t valid : 1;
	};
}

#endif //GAME_INDEX_H
