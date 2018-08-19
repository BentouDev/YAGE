#include "Utils/DefaultBlock.h"

namespace Memory
{
    namespace detail
    {
        IMemoryBlock& _getDefaultBlock()
        {
            static HeapAllocator              _defaultAllocator;
            static MemoryBlock<HeapAllocator> _defaultBlock(_defaultAllocator, "DEFAULT");
            return _defaultBlock;
        }
    }
}