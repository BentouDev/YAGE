//
// Created by mrjaqbq on 13.05.16.
//

#ifndef YAGE_INDEX_H
#define YAGE_INDEX_H

#include <cstdint>

namespace Utils
{
    template<typename Handle>
    struct Index
    {
        Handle   handle;
        uint16_t index;
        uint16_t next;
        uint8_t  valid : 1;
    };
}

#endif //YAGE_INDEX_H
