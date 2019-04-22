#include "Utils/MemoryBlock.h"
#include "Utils/DefaultBlock.h"

/*namespace eastl
{
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

	void* allocator::allocate(size_t n, int /flags/)
	{
		return Memory::detail::_getDefaultBlock().allocate(n, EASTL_SYSTEM_ALLOCATOR_MIN_ALIGNMENT, Utils::DebugSourceInfo{ "unknown", 0 });
	}

	void* allocator::allocate(size_t n, size_t alignment, size_t offset, int /flags/)
	{
		return Memory::detail::_getDefaultBlock().allocate(n, alignment, Utils::DebugSourceInfo{ "unknown", 0 });
	}

	void allocator::deallocate(void* p, size_t n)
	{
		Memory::detail::_getDefaultBlock().deallocate(p);
	}

	//Memory::MemoryBlockAllocator* gpSystemAllocator = nullptr;

	//template <>
	//inline Memory::MemoryBlockAllocator* get_default_allocator<Memory::MemoryBlockAllocator>(const Memory::MemoryBlockAllocator*)
	//{
	//	return gpSystemAllocator;
	//}
}*/
