//
// Created by bentoo on 21.11.16.
//

#ifndef GAME_TYPEINDEXLIST_H
#define GAME_TYPEINDEXLIST_H

#include <cstddef>
#include <type_traits>

namespace Utils
{
	template <typename ... Types>
	class TypeIndexList
	{
		struct indexValueHolder
		{
			bool exists;
			std::size_t index;
		};

		template <typename T>
		static constexpr indexValueHolder checkIndex(std::size_t counter = 0)
		{
			auto list = std::initializer_list<indexValueHolder>
			{
				indexValueHolder{std::is_same<Types, T>::value, counter++}...
			};
			for(auto& i : list)
			{
				if(i.exists)
					return i;
			}
			return indexValueHolder{false,std::numeric_limits<std::size_t>::max()};
		}

		template <typename T>
		class IndexOf
		{
		protected:
			static constexpr indexValueHolder value { checkIndex<T>() };
		public:
			static constexpr std::size_t index()
			{
				return value.index;
			}

			static constexpr bool exists()
			{
				return value.exists;
			}
		};

	public:
		template <typename T>
		static constexpr std::size_t indexOf()
		{
			static_assert(IndexOf<T>::exists(), "Type T must be in TypeIndexList parameters!");
			return IndexOf<T>::index();
		}

		template <typename T>
		static constexpr bool contains()
		{
			return IndexOf<T>::exists();
		}
	};
}

#endif //GAME_TYPEINDEXLIST_H
