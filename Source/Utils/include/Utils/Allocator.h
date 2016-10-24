//
// Created by bentoo on 9/26/16.
//

#ifndef GAME_ALLOCATOR_H
#define GAME_ALLOCATOR_H

#include <cstdint>
#include <memory>

namespace Memory
{
	const std::size_t ALLOCATION_SIZE_BYTES = 4;
	const std::size_t ADJUSTMENT_SIZE_BYTES = 1;

	class IAllocator
	{
	protected:
		void* _startPtr;

		std::size_t _size;
		std::size_t _usedSize;

		IAllocator(void* memory, std::size_t size) : _size(size), _usedSize(0), _startPtr(memory) { }

	public:
		virtual void* 		allocate(std::size_t size, std::size_t alignment, std::size_t offset) = 0;
		virtual void 		deallocate(void* ptr) = 0;
		virtual std::size_t getAllocationSize(const void* ptr) const = 0;
		virtual bool		hasAddress(const void* ptr) const = 0;

		bool hasSize(std::size_t size) const
		{
			return getFreeSize() > size;
		}

		void* getStart() const
		{
			return _startPtr;
		}

		void* getEnd() const
		{
			std::uintptr_t startAddress = reinterpret_cast<std::uintptr_t >(_startPtr);
			return reinterpret_cast<void*>(startAddress + _size);
		}

		std::size_t getSize() const
		{
			return _size;
		}

		std::size_t getUsedSize() const
		{
			return _usedSize;
		}

		std::size_t getFreeSize() const
		{
			return _size - _usedSize;
		}
	};

	namespace Internal
	{
		inline std::size_t calcAlignmentAdjustment(const void* ptr, size_t alignment)
		{
			std::size_t diff = reinterpret_cast<uintptr_t>(ptr) % alignment;
			if (diff != 0)
				return alignment - diff;
			return 0;
		}

		inline std::size_t calcForwardAlignmentAdjustment(const void *address, std::size_t alignment, std::size_t offset)
		{
			std::size_t adjustment  = calcAlignmentAdjustment(address, alignment);
			std::size_t neededSpace = offset;

			if(adjustment < neededSpace)
			{
				neededSpace -= adjustment;
				adjustment	+= alignment * (neededSpace / alignment);

				if(neededSpace % alignment > 0)
					adjustment += alignment;
			}

			return adjustment;
		}
	}
}

#endif //GAME_ALLOCATOR_H
