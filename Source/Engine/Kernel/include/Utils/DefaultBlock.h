#ifndef YAGE_DEFAULT_BLOCK_H
#define YAGE_DEFAULT_BLOCK_H

#include "MemoryBlock.h"
#include "DefaultAllocator.h"

namespace Memory
{
    namespace detail
    {
        IMemoryBlock& _getDefaultBlock();
    }

    template <typename T>
    IMemoryBlock& GetDefaultBlock()
    {
        return detail::_getDefaultBlock();
    }
}

#endif // YAGE_DEFAULT_BLOCK_H