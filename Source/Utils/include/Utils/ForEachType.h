//
// Created by bentoo on 22.11.16.
//

#ifndef GAME_FOREACHTYPE_H
#define GAME_FOREACHTYPE_H

#include <type_traits>

namespace Utils
{
	template <typename ... Types>
	class ForEachType
	{
		template <typename T>
		struct valueHome
		{
			static constexpr bool value[] = { std::is_base_of<T, Types>::value... };
		};

		template <typename T>
		static constexpr bool checkBaseOf(int count = 0)
		{
			return  count < sizeof...(Types) ? (
				valueHome<T>::value[count] ? checkBaseOf<T>(count + 1) : false
			) : true;
		}

		template <typename T>
		struct BaseOf
		{
			static constexpr bool are = checkBaseOf<T>();
		};

	public:
		template <typename T>
		static constexpr bool are_base_of()
		{
			return BaseOf<T>::are;
		}
	};
}

#endif //GAME_FOREACHTYPE_H
