//
// Created by mrjaqbq on 13.05.16.
//

#ifndef GAME_BORROWEDPTR_H
#define GAME_BORROWEDPTR_H

#include <algorithm>

namespace Utils
{
	template <typename T>
	class borrowed_ptr
	{
		T* value;

	public:
		explicit borrowed_ptr(T* raw_value = nullptr) noexcept :
		value { raw_value } { }

		borrowed_ptr(borrowed_ptr && other) noexcept : value { other.release() } { }

		borrowed_ptr(const borrowed_ptr& other) noexcept : value { other.getRaw() } { }

		auto operator=(borrowed_ptr && other) noexcept -> borrowed_ptr&
		{
			value = other.release();
			return *this;
		}

		auto operator=(const borrowed_ptr & other) noexcept -> borrowed_ptr&
		{
			value = other.getRaw();
			return *this;
		}

		auto reset(T* new_value = nullptr) noexcept -> bool
		{
			if(new_value != value)
			{
				value = new_value;
			}

			return static_cast<bool>(*this);
		}

		auto release() noexcept -> T*
		{
			auto raw_value = value;
			value = nullptr;
			return raw_value;
		}

		explicit operator bool() const noexcept
		{
			return value != nullptr;
		}

		auto operator->() noexcept -> T*
		{
			return getRaw();
		}

		auto operator->() const noexcept -> T*
		{
			return getRaw();
		}

		auto operator*() const noexcept -> T&
		{
			return get();
		}

		auto get() const noexcept -> T&
		{
			return *value;
		}

		auto getRaw() const noexcept -> T*
		{
			return value;
		}

		auto swap(borrowed_ptr<T>& other) noexcept -> void
		{
			std::swap(value, other.value);
		}

		static auto invalid() -> borrowed_ptr <T>
		{
			return borrowed_ptr<T>();
		}
	};

	template <typename T>
	auto swap(borrowed_ptr<T>& left, borrowed_ptr<T>& right) noexcept -> void
	{
		left.swap(right);
	}
}


#endif //GAME_BORROWEDPTR_H
