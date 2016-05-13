//
// Created by mrjaqbq on 13.05.16.
//

#ifndef GAME_INDEX_H
#define GAME_INDEX_H

namespace Utils
{
	template<typename Handle>
	struct Index
	{
		Handle handle;
		uint16_t index;
		uint16_t next;
	};
}

#endif //GAME_INDEX_H
