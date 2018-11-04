//
// Created by mrjaqbq on 15.05.16.
//

#ifndef YAGE_DEFAULTTRAIT_H
#define YAGE_DEFAULTTRAIT_H

#include <cstdint>
#include "Index.h"
#include "Handle.h"

namespace Utils
{
    /* ToDo: really use type traits?
       maybe introduce base class (SafeObject?) and put all that code there?
       Colony would require stored class to derive from it
    */
    template <typename T, typename THandle = Utils::Handle<T>>
    class DefaultTrait
    {
    public:
        using object_t = T;
        using handle_t = THandle;

        inline static void cleanUp(object_t& first)
        { first.cleanUp(); }

        inline static void swap(object_t& first, object_t& second) noexcept
        { first.swap(second); }

        inline static void incrementLiveId(Utils::Index<handle_t>& index) noexcept
        { index.handle.liveId++; }

        inline static void setIndex(Utils::Index<handle_t>& index, uint16_t i) noexcept
        { index.handle.index = i; }

        inline static uint16_t getIndex(Utils::Index<handle_t>& index) noexcept
        { return index.handle.index; }

        inline static uint16_t getIndex(handle_t _handle) noexcept
        { return _handle.index; }

        inline static void setHandle(object_t& obj, uint16_t liveId, uint32_t index) noexcept
        { obj.Handle.liveId = liveId; obj.Handle.index = index; }

        inline static handle_t getHandle(object_t& obj) noexcept
        { return obj.Handle; }
    };
}

#endif //YAGE_DEFAULTTRAIT_H
