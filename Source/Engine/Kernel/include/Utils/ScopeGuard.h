//
// Created by bentoo on 11.05.18.
//

#ifndef YAGE_DISPOSE_H
#define YAGE_DISPOSE_H

#include "MacroDefines.h"

namespace yage
{
    template <typename TFunc>
    struct scope_guard
    {
        operator =(TFunc&& func) noexcept
        {
            _func = std::move(func);
        }

        ~scope_guard()
        {
            _func();
        }

    private:
        TFunc _func;
    };

    template <typename TFunc>
    scope_guard(TFunc) -> scope_guard<TFunc>;
}

#define YAGE_DISPOSE [[maybe_unused]] yage::scope_guard YAGE_MAKE_UNIQUE(scope_exit_) = [&]()

#endif // YAGE_DISPOSE_H