#ifndef YAGE_DEFAULT_BLOCK_H
#define YAGE_DEFAULT_BLOCK_H

#include <Utils/MemoryBlock.h>
#include <Utils/HeapAllocator.h>

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