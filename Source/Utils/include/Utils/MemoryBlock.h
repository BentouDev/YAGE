//
// Created by bentoo on 9/27/16.
//

#ifndef GAME_MEMORYBLOCK_H
#define GAME_MEMORYBLOCK_H

#include <cstdint>
#include <cstring>

#include <new>
#include <type_traits>

#include "DebugSourceInfo.h"
#include "MemoryBoundChecker.h"
#include "MemoryTracker.h"

#define YAGE_CREATE_NEW(MemBlock, T) \
	new ( MemBlock.allocate ( sizeof ( T ) , alignof ( T ) , Utils::DebugSourceInfo ( __FILE__ , __LINE__ ) ) ) T

#define YAGE_CREATE_NEW_ARRAY(MemBlock, T, length) \
	Memory::CreateNewArray<decltype(MemBlock), T>(MemBlock, length, Utils::DebugSourceInfo(__FILE__, __LINE__))

#define YAGE_CREATE_NEW_ARRAY_DEF(MemBlock, T, length, defaultValue) \
	Memory::CreateNewArray<decltype(MemBlock), T>(MemBlock, length, defaultValue, Utils::DebugSourceInfo(__FILE__, __LINE__))

namespace Memory
{
	class IAllocator;

	class IMemoryBlock
	{
	protected:
		IMemoryBlock(){}
	private:
		IMemoryBlock(const IMemoryBlock&) = delete;
		IMemoryBlock(IMemoryBlock&&) = delete;
	public:
		virtual void* allocate(std::size_t size, std::size_t alignment, const Utils::DebugSourceInfo& sourceInfo) = 0;
		virtual void  deallocate(void* ptr) = 0;

		virtual std::size_t getFreeSize() = 0;
		virtual std::size_t getUsedSize() = 0;
		virtual std::size_t getCapacity() = 0;
	};

	// todo: mem checking, tracing
	template
	<	typename AllocatorType,
		typename MemoryBoundCheckerType = NoMemoryBoundChecker,
		typename MemoryTrackerType = NoMemoryTracker >
	class MemoryBlock final : public IMemoryBlock
	{
		static_assert
		(
			std::is_base_of<IAllocator, AllocatorType>::value,
			"AllocatorType must derive from IAllocator"
		);
		static_assert
		(
			std::is_base_of<IMemoryBoundChecker, MemoryBoundCheckerType>::value,
			"MemoryBoundCheckerType must derive from IMemoryBoundChecker"
		);
		static_assert
		(
			std::is_base_of<IMemoryTracker, MemoryTrackerType>::value,
			"MemoryTrackerType must derive from IMemoryTrackerType"
		);

		AllocatorType& 			_allocator;
		MemoryBoundCheckerType	_boundChecker;
		MemoryTrackerType		_memoryTracker;

	public:
		MemoryBlock(const MemoryBlock&) = delete;
		MemoryBlock(MemoryBlock&&) = delete;

		explicit MemoryBlock(AllocatorType& allocator)
			: _allocator(allocator), _memoryTracker(*this), IMemoryBlock()
		{

		}

		std::size_t getFreeSize() override
		{
			return _allocator.getFreeSize();
		}

		std::size_t getUsedSize() override
		{
			return _allocator.getUsedSize();
		}

		std::size_t getCapacity() override
		{
			return _allocator.getSize();
		}

		void* allocate(std::size_t size, std::size_t alignment, const Utils::DebugSourceInfo& sourceInfo) override
		{
			const std::size_t 	fronOffset		= _boundChecker.getSizeFront();
			const std::size_t 	backOffset 		= _boundChecker.getSizeFront();
			const std::size_t	originalSize	= size;
			const std::size_t	newSize 		= size + fronOffset + backOffset;
			void*				allocationPtr	= _allocator.allocate(newSize, alignment, fronOffset);

			std::uintptr_t allocationAddress = reinterpret_cast<std::uintptr_t>(allocationPtr);

			_boundChecker.GuardFront(allocationPtr);
			_boundChecker.GuardBack(reinterpret_cast<void*>(allocationAddress + fronOffset + originalSize));
			_memoryTracker.OnAllocation(allocationPtr, newSize, alignment, fronOffset, sourceInfo);

			return reinterpret_cast<void*>(allocationAddress + fronOffset);
		}

		void deallocate(void* ptr) override
		{
			const std::size_t 		frontOffset 	= _boundChecker.getSizeFront();
			const std::size_t 		backOffset 		= _boundChecker.getSizeFront();
			const std::uintptr_t 	originalAddress = reinterpret_cast<std::uintptr_t >(ptr) - frontOffset;
			void* 					originalPtr 	= reinterpret_cast<void*>(originalAddress);

			const std::size_t allocationSize = _allocator.getAllocationSize(originalPtr);

			_boundChecker.CheckFront(originalPtr);
			_boundChecker.CheckBack(reinterpret_cast<void*>(originalAddress + allocationSize - backOffset));
			_memoryTracker.OnDeallocation(originalPtr, frontOffset);

			_allocator.deallocate(originalPtr);

		//	std::memset(originalPtr, 0xCA, allocationSize);
		}
	};

	template <typename MemoryBlock, typename T>
	T* CreateNewArray(MemoryBlock& block, std::size_t length, const T& defaultValue, const Utils::DebugSourceInfo& source)
	{
		std::size_t memory_size = sizeof(T) * length + sizeof(std::size_t);
		union {
			void* as_raw_ptr;
			std::size_t* as_size;
			T* as_T;
		} memory;

		memory.as_raw_ptr = block.allocate(memory_size, alignof(T), source);
		*memory.as_size++ = length;

		for(const T* const endPtr = memory.as_T + length; memory.as_T < endPtr;)
			new (memory.as_T++) T(defaultValue);

		return memory.as_T - length;
	}

	template <typename MemoryBlock, typename T>
	T* CreateNewArray(MemoryBlock& block, std::size_t length, const Utils::DebugSourceInfo& source)
	{
		std::size_t memory_size = sizeof(T) * length + sizeof(std::size_t);
		union {
			void* as_raw_ptr;
			std::size_t* as_size;
			T* as_T;
		} memory;

		 memory.as_raw_ptr = block.allocate(memory_size, alignof(T), source);
		*memory.as_size++ = length;

		for(const T* const endPtr = memory.as_T + length; memory.as_T < endPtr;)
			new (memory.as_T++) T;

		return memory.as_T - length;
	}

	template <typename MemoryBlock, typename T>
	void Delete(MemoryBlock& block, T*& ptr)
	{
		if(ptr != nullptr)
		{
			ptr->~T();
			block.deallocate(ptr);
			ptr = nullptr;
		}
	}

	template <typename MemoryBlock, typename T>
	void DeleteArray(MemoryBlock& block, T*& ptr)
	{
		if(ptr != nullptr)
		{
			T* currentPtr = ptr;
			std::uintptr_t 	lengthAddress 	=  reinterpret_cast<std::uintptr_t >(ptr) - sizeof(std::size_t);
			std::size_t 	length 			= *reinterpret_cast<std::size_t*>(lengthAddress);

			for(const T* const endPtr = ptr + length; currentPtr < endPtr;)
				currentPtr++->~T();

			block.deallocate(ptr);
			ptr = nullptr;
		}
	}
}

#endif //GAME_MEMORYBLOCK_H
