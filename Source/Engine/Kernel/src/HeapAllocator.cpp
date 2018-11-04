#include <Utils/DefaultAllocator.h>

namespace Memory
{
    HeapAllocator::HeapAllocator()
        : IAllocator(nullptr, 0)
    {

    }

    HeapAllocator::~HeapAllocator()
    {

    }

    std::size_t HeapAllocator::getAllocationSize(const void* ptr) const
    {
        std::uintptr_t 	allocationSizeAddress	= reinterpret_cast<std::uintptr_t>(ptr) - ALLOCATION_SIZE_BYTES;
        std::size_t 	allocationSize			= *reinterpret_cast<std::size_t*> (allocationSizeAddress);
        return allocationSize;
    }

    void* HeapAllocator::allocate(std::size_t size, std::size_t alignment, std::size_t offset)
    {
        std::size_t headerSize = ALLOCATION_SIZE_BYTES + ADJUSTMENT_SIZE_BYTES + offset;
		char*       raw_memory = reinterpret_cast<char*>(malloc(size + headerSize + alignment));
        std::size_t adjustment = Internal::calcForwardAlignmentAdjustment(raw_memory, alignment, headerSize);

        std::uintptr_t currentAddress = reinterpret_cast<std::uintptr_t >(raw_memory);
        std::uintptr_t alignedAddress = currentAddress + adjustment;
        std::uintptr_t adjustmentSize = alignedAddress - headerSize;
        std::uintptr_t allocationSize = adjustmentSize + ADJUSTMENT_SIZE_BYTES;

        void* allocation = reinterpret_cast<void*>(alignedAddress - offset);

        *(reinterpret_cast<std::size_t*> (allocationSize)) = ((std::size_t)	 size);
        *(reinterpret_cast<std::uint8_t*>(adjustmentSize)) = ((std::uint8_t) adjustment - offset);

        return allocation;
    }

    void  HeapAllocator::deallocate(void* ptr)
    {
        std::uintptr_t  address 	          =  reinterpret_cast<std::uintptr_t>(ptr);
        std::uintptr_t 	allocationSizeAddress =  reinterpret_cast<std::uintptr_t>(ptr) - ALLOCATION_SIZE_BYTES;
        std::size_t 	allocationSize		  = *reinterpret_cast<std::size_t*>  (allocationSizeAddress);

		std::uintptr_t 	adjustmentSizeAddress = allocationSizeAddress - ADJUSTMENT_SIZE_BYTES;
		uint8_t 		adjustmentSize        = *reinterpret_cast<std::uint8_t*>(adjustmentSizeAddress);

        void*           allocation            =  reinterpret_cast<void*>(address - adjustmentSize);

        free (allocation);
    }

    bool HeapAllocator::hasAddress(const void* ptr) const
    {
        return false;
    }
}
