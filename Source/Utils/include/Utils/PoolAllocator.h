//
// Created by bentoo on 9/28/16.
//

#ifndef GAME_POOLALLOCATOR_H
#define GAME_POOLALLOCATOR_H

#include "Allocator.h"

namespace Memory
{
	class PoolAllocator : public IAllocator
	{
	private:
		PoolAllocator(const PoolAllocator&) = delete;
		PoolAllocator(PoolAllocator&&) = delete;

		std::size_t _alignOffset;
		std::size_t _typeSize;
		std::size_t _typeAlignment;

		void** _freePtr;

	public:
		PoolAllocator(void *memory, std::size_t size,
	  		std::size_t typeSize, std::size_t typeAlign, std::size_t alignOffset);

		template <typename T>
		static PoolAllocator* create(void* poolPtr, void* memory, std::size_t size, std::size_t offset);

		void* 		allocate(std::size_t size, std::size_t alignment, std::size_t offset) override;
		void 		deallocate(void* ptr) override;
		std::size_t getAllocationSize(const void* ptr) const override;
		bool 		hasAddress(const void* ptr) const override;
	};

	template <typename T>
	PoolAllocator* PoolAllocator::create(void* poolPtr, void* memory, std::size_t size, std::size_t offset)
	{
		return new (poolPtr) PoolAllocator(memory, size, sizeof(T), alignof(T), offset);
	}
}

#endif //GAME_POOLALLOCATOR_H
