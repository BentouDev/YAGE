//
// Created by mrjaqbq on 15.05.16.
//

#ifndef YAGE_DEFAULTTRAIT_H
#define YAGE_DEFAULTTRAIT_H

#include <cstdint>
#include "Handle.h"

namespace Utils
{
    /* ToDo: really use type traits?
       maybe introduce base class (SafeObject?) and put all that code there?
       Colony would require stored class to derive from it
    */
    template <typename T>
    class SafeObjectTrait
    {
        static_assert(std::is_base_of<yage::SafeObject, T>::value, "SafeObjectTrait can only be used for classes derived from SafeObject!");
 
    public:
        using object_t = T;
        using handle_t = Utils::Handle<T>;

        inline static uint16_t getIndex(handle_t _handle) noexcept
        {
            return _handle.key.index;
        }

        inline static void setHandle(object_t& obj, uint16_t liveId, uint32_t index) noexcept
        {
            // ToDo: remove this from here, it's slow...
            if (!obj.Handle.key.typeId)
                obj.Handle.key.typeId = handle_t().key.typeId;

            obj.Handle.key.liveId = liveId;
            obj.Handle.key.index = index;
        }

        inline static handle_t getHandle(object_t& obj) noexcept
        {
            return Utils::handle_cast<T>(obj.Handle);
        }
    };
}

#endif //YAGE_DEFAULTTRAIT_H
