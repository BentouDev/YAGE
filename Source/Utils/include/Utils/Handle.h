//
// Created by mrjaqbq on 06.03.16.
//

#ifndef YAGE_HANDLE_H
#define YAGE_HANDLE_H

#include <cstdint>
#include <utility>
#include "TypeInfo.h"
#include "Assert.h"

namespace Utils
{
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

	template <typename Resource>
	union Handle
	{
	public:
		uint32_t key;

		struct
		{
			uint8_t liveId;
			type_t typeId;
			uint16_t index;
		};

	private:
		auto release() -> uint32_t
		{
			auto oldVal = key;
			key = invalid().key;
			return oldVal;
		}
	public:

		Handle() : key(0) { typeId = TypeInfo<Resource>::id(); }
		Handle(Handle&& other) : key(other.release()) { }
		Handle(const Handle& other) : key(other.key) { }

		auto operator=(const Handle& other) -> Handle& { key = other.key; return *this; }
		auto operator=(Handle&& other) -> Handle& { key = other.release(); return *this; }

		operator RawHandle()
		{
			return RawHandle(key);
		}

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

	template <typename Handle>
	Handle handle_cast(RawHandle handle)
	{
		Handle h;

		YAGE_ASSERT(h.typeId == handle.typeId,
					"handle_cast : Unable to cast handles from '%d' to '%d'!",
					h.typeId, h.typeId);

		h.liveId	= handle.liveId;
		h.index		= handle.index;

		return h;
	}
}

#endif //YAGE_HANDLE_H
