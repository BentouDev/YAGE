#ifndef YAGE_HEAP_ALLOCATOR_H
#define YAGE_HEAP_ALLOCATOR_H

#include <Utils/Allocator.h>

namespace Memory
{
    class HeapAllocator : public IAllocator
    {
	private:
		HeapAllocator(const HeapAllocator&) = delete;
		HeapAllocator(HeapAllocator&&) = delete;

    public:
         HeapAllocator();
		~HeapAllocator();

		void* 		allocate(std::size_t size, std::size_t alignment, std::size_t offset) override;
		void 		deallocate(void* ptr) override;
		std::size_t getAllocationSize(const void* ptr) const override;
		bool 		hasAddress(const void* ptr) const override;
    };
}

#endif // !YAGE_HEAP_ALLOCATOR_H