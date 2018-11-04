#include "Utils/DefaultBlock.h"
#include "Utils/SimpleMemoryBoundChecker.h"

namespace Memory
{
    namespace detail
    {
		using TAllocator = HeapAllocator;
		using TMemoryBlock = MemoryBlock <TAllocator>;
		// using TMemoryBlock = MemoryBlock <TAllocator, Memory::SimpleMemoryBoundChecker>;

		IMemoryBlock& _getDefaultBlock()
        {
            static TAllocator    _defaultAllocator;
            static TMemoryBlock  _defaultBlock(_defaultAllocator, "DEFAULT");
            return _defaultBlock;
        }
    }
}