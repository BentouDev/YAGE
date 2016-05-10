//
// Created by mrjaqbq on 06.03.16.
//

#ifndef VOLKHVY_HANDLE_H
#define VOLKHVY_HANDLE_H

#include <cstdint>
#include <utility>

namespace Utils
{
	typedef uint32_t handle_t;

	union RawHandle
	{

	public:
		handle_t Key;

		struct {
			uint8_t LiveId;
			uint8_t Type;
			uint16_t Index;
		};

		RawHandle() : Key(0) {}
	};

	template<typename Traits>
	class Handle
	{
	public:
		using ptr_t = typename Traits::ptr_t;

	private:

		ptr_t value;

		auto close() noexcept -> void
		{
			if(*this)
			{
				Traits::close(value);
			}
		}

		Handle(Handle const &) = delete;
		auto operator=(Handle const&) -> Handle& = delete;

	public:

		explicit Handle(ptr_t raw_value = Traits::invalid()) noexcept :
			value { raw_value } { }

		Handle(Handle && other) noexcept : value { other.release() } { }

		auto operator=(Handle && other) noexcept -> Handle&
		{
			if(this != &other)
			{
				reset(other.release());
			}

			return *this;
		}

		~Handle() noexcept
		{
			close();
		}

		explicit operator bool() const noexcept
		{
			return value != Traits::invalid();
		}

		auto get() const noexcept -> ptr_t
		{
			return value;
		}

		auto release() noexcept -> ptr_t
		{
			auto raw_value = value;
			value = Traits::invalid();
			return raw_value;
		}

		auto reset(ptr_t raw_value = Traits::invalid()) noexcept -> bool
		{
			if(value != raw_value)
			{
				close();
				value = raw_value;
			}

			return static_cast<bool>(*this);
		}

		auto swap(Handle<Traits>& other) noexcept -> void
		{
			std::swap(value, other.value);
		}
	};

	template<typename Traits>
	auto swap(Handle<Traits>& left, Handle<Traits>& right) noexcept -> void
	{
		left.swap(right);
	}

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

#endif //VOLKHVY_HANDLE_H
