//
// Created by mrjaqbq on 06.03.16.
//

#ifndef YAGE_HANDLE_H
#define YAGE_HANDLE_H

#include <cstdint>
#include <utility>
#include "TypeInfo.h"

namespace Utils
{
	template <typename Resource>
	union Handle
	{
	private:
		auto release() -> uint32_t
		{
			auto oldVal = key;
			key = invalid().key;
			return oldVal;
		}

	public:
		uint32_t key;

		struct
		{
			uint8_t liveId;
			type_t typeId;
			uint16_t index;
		};

		Handle() : key(0) { typeId = TypeInfo<Resource>::id(); }
		Handle(Handle&& other) : key(other.release()) { }
		Handle(const Handle& other) : key(other.key) { }

		auto operator=(const Handle& other) -> Handle& { key = other.key; return *this; }
		auto operator=(Handle&& other) -> Handle& { key = other.release(); return *this; }

		static auto invalid() -> Handle<Resource>
		{
			return Handle();
		}

		explicit operator bool() const
		{
			return key != invalid().key;
		}

		auto operator==(const Handle<Resource>& other) -> bool
		{
			return key == other.key;
		}

		auto operator!=(const Handle<Resource>& other) -> bool
		{
			return key != other.key;
		}

		auto operator==(const Handle<Resource>& other) const -> bool
		{
			return key == other.key;
		}

		auto operator!=(const Handle<Resource>& other) const -> bool
		{
			return key != other.key;
		}

		auto swap(Handle<Resource>& other) noexcept -> void
		{
			std::swap(key, other.key);
		}
	};

	template<typename Resource>
	auto swap(Handle<Resource>& left, Handle<Resource>& right) noexcept -> void
	{
		left.swap(right);
	}

	union RawHandle
	{
	public:
		RawHandle() : key(0)
		{ }

		RawHandle(uint32_t new_key) : key(new_key)
		{ }

		uint32_t key;

		struct
		{
			uint8_t liveId;
			type_t typeId;
			uint16_t index;
		};

		static auto invalid() -> RawHandle
		{
			return RawHandle();
		}

		operator bool()
		{
			return key != invalid().key;
		}
	};
	/*typedef uint32_t handle_t;

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
	}*/
}

#endif //YAGE_HANDLE_H
