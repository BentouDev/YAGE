//
// Created by bentoo on 21.11.16.
//

#ifndef GAME_TYPEINDEXLIST_H
#define GAME_TYPEINDEXLIST_H

#include <cstddef>
#include <limits>

namespace Utils
{
	template <typename ... Types>
	class TypeIndexList
	{
		template <typename T>
		struct valueHome
		{
			static constexpr bool value[] = { std::is_same<T, Types>::value... };
		};

		template <typename T>
		static constexpr std::size_t checkIndex(std::size_t count = 0)
		{
			return count < sizeof...(Types) ? (
				valueHome<T>::value[count] ? count : checkIndex<T>(count + 1)
			) : std::numeric_limits<std::size_t>::max();
		}

		template <typename T>
		struct IndexOf
		{
			static constexpr std::size_t index = checkIndex<T>();
			static constexpr bool exists()
			{
				return index < sizeof...(Types);
			}
		};

	public:
		template <typename T>
		static constexpr bool contains()
		{
			return IndexOf<T>::exists();
		}

		template <typename T>
		static constexpr std::size_t indexOf()
		{
			static_assert(IndexOf<T>::exists(), "Type T must be in TypeIndexList parameters!");
			return IndexOf<T>::index;
		}
	};
}

#endif //GAME_TYPEINDEXLIST_H
