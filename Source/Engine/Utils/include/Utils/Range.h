//
// Created by Bentoo on 2017-02-11.
//

#ifndef GAME_RANGE_H
#define GAME_RANGE_H

#include <algorithm>

namespace Utils
{
	template <typename T, T min, T max>
	class Range
	{
		T _value;

	public:
		explicit Range() : _value(std::min(max, std::max(min, T())))
		{ }

		explicit Range(const T& value) : _value(std::min(max, std::max(min, value)))
		{ }

		explicit Range(T&& value) : _value(std::min(max, std::max(min, value)))
		{ }

		Range& operator=(const T& other)
		{
			_value = std::min(max, std::max(min, other));
			return *this;
		}

		Range& operator=(T&& other)
		{
			_value = std::min(max, std::max(min, other));
			return *this;
		}

		operator T()
		{
			return _value;
		}
	};
}

#endif //GAME_RANGE_H
