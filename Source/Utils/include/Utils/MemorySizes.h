//
// Created by bentoo on 11/3/16.
//

#ifndef GAME_MEMORYSIZES_H
#define GAME_MEMORYSIZES_H

#include <cstddef>

namespace Memory
{
	template <typename T>
	constexpr std::size_t KB(T x)
	{
		return ((std::size_t)(x) << 10);
	}

	template <typename T>
	constexpr std::size_t MB(T x)
	{
		return ((std::size_t)(x) << 20);
	}
}

#endif //GAME_MEMORYSIZES_H
