//
// Created by MrJaqbq on 2016-03-11.
//

#ifndef VOLKHVY_BASEAPI_H
#define VOLKHVY_BASEAPI_H

namespace Gfx
{
    template<typename Implementation>
    class BaseApi
    {
        inline auto initialize() noexcept -> bool
        {
            static_cast<Implementation*>(this)->initialize();
        }
    };
}

#endif //VOLKHVY_BASEAPI_H
