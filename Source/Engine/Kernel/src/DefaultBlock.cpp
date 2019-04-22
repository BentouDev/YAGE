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

// #NewAlloc
// For eastl
//void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
//{
//	return Memory::detail::_getDefaultBlock().allocate(size, EASTL_ALLOCATOR_MIN_ALIGNMENT, Utils::DebugSourceInfo{file, (size_t) line});
//}
//
//// But what with delete?
//void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
//{
//	YAGE_ASSERT(alignmentOffset == 0 && alignment == 0, "Alignment and offset are not supported yet! Delete is not overloaded!");
//	return Memory::detail::_getDefaultBlock().allocate(size, 1, Utils::DebugSourceInfo{ file, (size_t) line });
//}