//
// Created by mrjaqbq on 06.03.16.
//

#ifndef YAGE_HANDLE_H
#define YAGE_HANDLE_H

#include <cstdint>
#include <utility>
#include <RTTI/ClassInfo.h>
#include <RTTI/Reflection.h>
#include "Assert.h"

namespace Utils
{
    struct RawHandle
    {
    public:
        RawHandle() : key(0)
        { }

        RawHandle(uint32_t new_key) : key(new_key)
        { }

		union
		{
			uint32_t key = { 0 };

			struct
			{
				uint8_t liveId;
				type_t typeId;
				uint16_t index;
			};
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
    struct Handle
    {
    public:
		union
		{
			uint32_t key;

			// ToDo : Make handle 64bit
			struct
			{
				uint8_t  liveId;
				type_t   typeId;
				uint16_t index;
			};
		};
        
    private:
        auto release() -> uint32_t
        {
            auto oldVal = key;
            key = invalid().key;
            return oldVal;
        }
    public:

        Handle() : key(0) 
        {
            // ToDo : provide a way to get just TypeInfo*
            auto* type = Meta::GetType<Resource>();
            YAGE_ASSERT(type != nullptr, "Cannot create handle for non-registered type!");
            typeId = type != nullptr ? type->GetId() : 0; 
        }

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

	template <typename T>
	Utils::Handle<T> handle_cast(RawHandle handle)
	{
		Utils::Handle<T> h;

		YAGE_ASSERT(h.typeId == handle.typeId,
			"handle_cast : Unable to cast handles from '{}' to '{}'!",
			h.typeId, h.typeId);

		h.liveId = handle.liveId;
		h.index = handle.index;

		return h;
	}
}

#endif //YAGE_HANDLE_H
