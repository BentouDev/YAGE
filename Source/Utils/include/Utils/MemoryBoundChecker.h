//
// Created by bentoo on 10/21/16.
//

#ifndef GAME_MEMORYBOUNDCHECKER_H
#define GAME_MEMORYBOUNDCHECKER_H

#include <cstdlib>

namespace Memory
{
	class IMemoryBlock;

	class IMemoryBoundChecker
	{
	protected:
		Memory::IMemoryBlock& _block;
		explicit IMemoryBoundChecker(Memory::IMemoryBlock& memory)
			: _block(memory)
		{ }

		void* getSuperblockPtr() const;
		const char* getName() const;

	public:
		virtual inline std::size_t getSizeFront() const = 0;
		virtual inline std::size_t getSizeBack() const = 0;

		virtual inline void GuardFront(void*) = 0;
		virtual inline void GuardBack(void*) = 0;

		virtual inline void CheckFront(const void*) = 0;
		virtual inline void CheckBack(const void*) = 0;
	};

	class NoMemoryBoundChecker : public IMemoryBoundChecker
	{
	public:
		explicit NoMemoryBoundChecker(Memory::IMemoryBlock& memory)
			: IMemoryBoundChecker(memory)
		{ }

		inline std::size_t getSizeFront() const override
		{ return 0; };

		inline std::size_t getSizeBack() const override
		{ return 0; };

		inline void GuardFront(void*) override {}
		inline void GuardBack(void*) override {}

		inline void CheckFront(const void*) override {}
		inline void CheckBack(const void*) override {}
	};
}

#endif //GAME_MEMORYBOUNDCHECKER_H
