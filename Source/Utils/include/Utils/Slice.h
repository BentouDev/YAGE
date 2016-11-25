//
// Created by bentoo on 25.11.16.
//

#ifndef GAME_SLICE_H
#define GAME_SLICE_H

#include <cstddef>

namespace Utils
{
	template <typename T>
	class Slice
	{
		T*			_begin;
		std::size_t	_size;

	public:
		Slice(T* ptr, std::size_t size)
			: _begin(ptr), _size(size)
		{ }

		Slice(T* begin, T* end)
			: _begin(begin), _size(end - begin)
		{ }

		~Slice()
		{
			// Slice is not freeing its resources!
		}

		inline auto operator[](std::size_t index) -> T&
		{
			return _begin[index];
		}

		inline auto operator[](std::size_t index) const -> const T&
		{
			return _begin[index];
		}

		auto front() -> T&
		{
			return _begin[0];
		}

		auto back() -> T&
		{
			return _begin[_size - 1];
		}

		auto front() const -> T&
		{
			return _begin[0];
		}

		auto back() const -> T&
		{
			return _begin[_size - 1];
		}

		auto begin() -> T*
		{
			return _begin;
		}

		auto end() -> T*
		{
			return _begin + _size;
		}

		auto begin() const -> T*
		{
			return _begin;
		}

		auto end() const -> T*
		{
			return _begin + _size;
		}

		inline auto size() const noexcept -> std::size_t
		{
			return _size;
		}
	};
}

#endif //GAME_SLICE_H
