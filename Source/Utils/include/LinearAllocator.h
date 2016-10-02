//
// Created by bentoo on 9/27/16.
//

#ifndef GAME_LINEARALLOCATOR_H
#define GAME_LINEARALLOCATOR_H

#include "Allocator.h"

namespace Memory
{
	class LinearAllocator : public Allocator
	{
	private:
		LinearAllocator(const LinearAllocator&) = delete;
		LinearAllocator(LinearAllocator&&) = delete;

		void* _currentPtr;

	public:
		 LinearAllocator(void* memory, std::size_t size);
		~LinearAllocator();

		void clear();

		void* 		allocate(std::size_t size, std::size_t alignment, std::size_t offset) override;
		void*		resize(void* ptr, std::size_t newSize) override;
		void  		deallocate(void* ptr) override;
		std::size_t getAllocationSize(const void* ptr) const override;
		bool 		hasAddress(const void* ptr) const override;
	};
}

#endif //GAME_LINEARALLOCATOR_H
