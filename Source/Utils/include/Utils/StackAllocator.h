//
// Created by bentoo on 9/28/16.
//

#ifndef GAME_STACKALLOCATOR_H
#define GAME_STACKALLOCATOR_H

#include "Allocator.h"

namespace Memory
{
	class StackAllocator : public IAllocator
	{
	private:
		StackAllocator(const StackAllocator&) = delete;
		StackAllocator(StackAllocator&&) = delete;

		void* _currentPtr;
		void* _lastAllocation;

	public:
		 StackAllocator(void* memory, std::size_t);
		~StackAllocator();

		void* 		allocate(std::size_t size, std::size_t alignment, std::size_t offset) override;
		void 		deallocate(void* ptr);
		std::size_t getAllocationSize(const void* ptr) const override;
		bool 		hasAddress(const void* ptr) const override;
	};
}

#endif //GAME_STACKALLOCATOR_H
