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

namespace eastl
{
	inline bool operator==(const allocator&, const allocator&)
	{ return true; }

	inline allocator::allocator(const char* EASTL_NAME(pName))
	{
#if EASTL_NAME_ENABLED
		mpName = pName ? pName : EASTL_ALLOCATOR_DEFAULT_NAME;
#endif
	}


	inline allocator::allocator(const allocator& EASTL_NAME(alloc))
	{
#if EASTL_NAME_ENABLED
		mpName = alloc.mpName;
#endif
	}


	inline allocator::allocator(const allocator&, const char* EASTL_NAME(pName))
	{
#if EASTL_NAME_ENABLED
		mpName = pName ? pName : EASTL_ALLOCATOR_DEFAULT_NAME;
#endif
	}


	inline allocator& allocator::operator=(const allocator& EASTL_NAME(alloc))
	{
#if EASTL_NAME_ENABLED
		mpName = alloc.mpName;
#endif
		return *this;
	}


	inline const char* allocator::get_name() const
	{
#if EASTL_NAME_ENABLED
		return mpName;
#else
		return EASTL_ALLOCATOR_DEFAULT_NAME;
#endif
	}


	inline void allocator::set_name(const char* EASTL_NAME(pName))
	{
#if EASTL_NAME_ENABLED
		mpName = pName;
#endif
	}

	inline void* allocator::allocate(size_t n, int /*flags*/)
	{
		return Memory::detail::_getDefaultBlock().allocate(n, EASTL_SYSTEM_ALLOCATOR_MIN_ALIGNMENT, Utils::DebugSourceInfo{ "unknown", 0 });
	}

	inline void* allocator::allocate(size_t n, size_t alignment, size_t offset, int /*flags*/)
	{
		return Memory::detail::_getDefaultBlock().allocate(n, alignment, Utils::DebugSourceInfo{ "unknown", 0 });
	}

	inline void allocator::deallocate(void* p, size_t n)
	{
		Memory::detail::_getDefaultBlock().deallocate(p);
	}
}

#endif // YAGE_DEFAULT_BLOCK_H