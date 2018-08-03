//
// Created by bentoo on 14.05.18.
//

#ifndef YAGE_RELATIVEPTR_H
#define YAGE_RELATIVEPTR_H

#include <cstdint>
#include <limits>
#include "Assert.h"

namespace Utils
{
    template <typename TClass, typename TPtr = std::int32_t>
    class relative_ptr
    {
        TPtr value;

        void internal_assign(std::int64_t diff) {
            if (diff > std::numeric_limits<TPtr>::max() || diff < std::numeric_limits<TPtr>::min())
            {
                YAGE_ASSERT(false, "Numerical overflow on relative ptr {} of diff {}", (void*)this, diff);
                value = 0;
            }
            else
                value = (TPtr) diff;
        }

    public:
        relative_ptr()
            : value(0)
        { }

        explicit relative_ptr(TClass* ptr) {
            internal_assign(reinterpret_cast<char*>(ptr) - reinterpret_cast<char*>(this));
        }

        relative_ptr(const relative_ptr& copy) {
            internal_assign((reinterpret_cast<char*>(&copy) + copy.value) - reinterpret_cast<char*>(this));
        }

        relative_ptr(relative_ptr&& other) noexcept {
            internal_assign((reinterpret_cast<char*>(&other) + other.value) - reinterpret_cast<char*>(this));
        }

        relative_ptr& operator=(const relative_ptr& copy) {
            if (this != &copy) {
                internal_assign((reinterpret_cast<char*>(&copy) + copy.value) - reinterpret_cast<char*>(this));
            }

            return *this;
        }

        relative_ptr& operator=(relative_ptr&& other) noexcept {
            if (this != &other) {
                internal_assign((reinterpret_cast<char*>(&other) + other.value) - reinterpret_cast<char*>(this));
            }

            return *this;
        }

        relative_ptr& operator=(std::nullptr_t) {
            value = 0;
            return *this;
        }

        relative_ptr& operator=(relative_ptr* ptr) {
            if (this != ptr) {
                internal_assign((reinterpret_cast<char*>(ptr) + ptr->value) - reinterpret_cast<char*>(this));
            } else {
                value = std::numeric_limits<TPtr>::min();
            }

            return *this;
        }

        relative_ptr& operator=(const TClass* ptr) {
            if (this == reinterpret_cast<const relative_ptr*>(ptr))
                value = std::numeric_limits<TPtr>::min();
            else
                internal_assign(reinterpret_cast<const char*>(ptr) - reinterpret_cast<const char*>(this));
            return *this;
        }

        TClass& operator*() const {
            auto* self = const_cast<relative_ptr*>(this);
            if (value == std::numeric_limits<TPtr>::min())
                return *reinterpret_cast<TClass*>(self);
            return *(reinterpret_cast<TClass*>(reinterpret_cast<char*>(self) + value));
        }

        TClass* operator->() const {
            auto* self = const_cast<relative_ptr*>(this);
            if (value == std::numeric_limits<TPtr>::min())
                return reinterpret_cast<TClass*>(self);
            YAGE_ASSERT(value != 0, "Relative ptr is null!");
            return value == 0 ? nullptr : reinterpret_cast<TClass*>(reinterpret_cast<char*>(self) + value);
        }

        bool operator==(TClass* ptr) const {
            return this->operator->() == ptr;
        }

        bool operator==(relative_ptr* other) const {
            return this->operator->() == other->operator->();
        }

        bool operator==(const relative_ptr& other) const {
            return this->operator->() == other.operator->();
        }

        bool operator==(std::nullptr_t) const {
            return value == 0;
        }

        bool operator!=(std::nullptr_t) const {
            return value != 0;
        }

        explicit operator bool() const {
            return value != 0;
        }
    };
}

#endif //YAGE_RELATIVEPTR_H
