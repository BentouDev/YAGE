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
    union handle_t
    {
        handle_t(uint64_t data)
            : raw(data)
        { }

        uint64_t raw = { 0 };

        struct
        {
            uint16_t liveId;
            type_t   typeId;
            uint32_t index;
        };
    };

    struct RawHandle
    {
    public:
        RawHandle() : key(0)
        { }

        RawHandle(uint64_t new_key) : key(new_key)
        { }

        handle_t key;
        
        static auto invalid() -> RawHandle
        {
            return RawHandle();
        }

        operator bool()
        {
            return key.raw != invalid().key.raw;
        }
    };

    template <typename Resource>
    struct Handle
    {
    public:
        handle_t key;

    private:
        auto release() -> uint64_t
        {
            auto oldVal = key.raw;
            key.raw = invalid().key.raw;
            return oldVal;
        }
    public:

        Handle() : key(0)
        {
            // ToDo : provide a way to get just TypeInfo*
            RTTI::ClassInfo* type = Meta::GetType<Resource>();
            YAGE_ASSERT(type != nullptr, "Cannot create handle for non-registered type!");
            key.typeId = type != nullptr ? type->GetId() : 0; 
        }

        Handle(Handle&& other) : key(other.release()) { }
        Handle(const Handle& other) : key(other.key) { }

        auto operator=(const Handle& other) -> Handle& { key = other.key; return *this; }
        auto operator=(Handle&& other) -> Handle& { key = other.release(); return *this; }

        operator RawHandle()
        {
            return RawHandle(key.raw);
        }

        static auto invalid() -> Handle<Resource>
        {
            return Handle();
        }

        explicit operator bool() const
        {
            return key.raw != invalid().key.raw;
        }

        auto operator==(const Handle<Resource>& other) -> bool
        {
            return key.raw == other.key.raw;
        }

        auto operator!=(const Handle<Resource>& other) -> bool
        {
            return key.raw != other.key.raw;
        }

        auto operator==(const Handle<Resource>& other) const -> bool
        {
            return key.raw == other.key.raw;
        }

        auto operator!=(const Handle<Resource>& other) const -> bool
        {
            return key.raw != other.key.raw;
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
    Utils::Handle<T> handle_cast(const RawHandle& handle)
    {
        Utils::Handle<T> h;

        YAGE_ASSERT(h.key.typeId == handle.key.typeId,
            "handle_cast : Unable to cast handles from '{}' to '{}'!",
            h.key.typeId, h.key.typeId);

        h.key.liveId = handle.key.liveId;
        h.key.index = handle.key.index;

        return h;
    }
}

#endif //YAGE_HANDLE_H
