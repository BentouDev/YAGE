//
// Created by mrjaqbq on 13.05.16.
//

#ifndef GAME_INDEX_H
#define GAME_INDEX_H

#include <stdint.h>

namespace Utils
{
	template<typename Handle>
	struct Index
	{
	public:
		Handle handle;
		uint16_t index;
		uint16_t next;
		uint8_t active : 1;
		uint8_t valid : 1;
	};
}

#endif //GAME_INDEX_H
