//
// Created by bentoo on 05.11.16.
//

#ifndef GAME_SIMPLEMEMORYBOUNDCHECKER_H
#define GAME_SIMPLEMEMORYBOUNDCHECKER_H

#include <cstdint>
#include <cstdio>
#include "MemoryBoundChecker.h"

namespace Memory
{
    class SimpleMemoryBoundChecker : public IMemoryBoundChecker
    {

    public:
        explicit SimpleMemoryBoundChecker(IMemoryBlock& memory)
            : IMemoryBoundChecker(memory)
        { }

        const static std::uint32_t MAGIC_NUMBER_FRONT = 0xDEADBEFF;
        const static std::uint32_t MAGIC_NUMBER_BACK = 0xBAADF00D;
        const static std::size_t BOUND_OFFSET_FRONT = sizeof(MAGIC_NUMBER_FRONT);
        const static std::size_t BOUND_OFFSET_BACK = sizeof(MAGIC_NUMBER_BACK);

        inline std::size_t getSizeFront() const override
        {
            return BOUND_OFFSET_FRONT;
        };

        inline std::size_t getSizeBack() const override
        {
            return BOUND_OFFSET_BACK;
        };

        inline void GuardFront(void* ptr) override
        {
            std::uint32_t* guardPtr = reinterpret_cast<std::uint32_t*>(ptr);
            (*guardPtr) = MAGIC_NUMBER_FRONT;
        }

        inline void GuardBack(void* ptr) override
        {
            std::uint32_t* guardPtr = reinterpret_cast<std::uint32_t*>(ptr);
            (*guardPtr) = MAGIC_NUMBER_BACK;
        }

        inline void CheckFront(const void* ptr) override
        {
            const std::uint32_t* guardPtr = reinterpret_cast<const std::uint32_t*>(ptr);
            const std::uint32_t		guardValue = *guardPtr;
            if (guardValue != MAGIC_NUMBER_FRONT)
            {
                std::fprintf(stderr, "%s : Front guard damaged for address '%p', was '%zu'\n",
                    getName(), ptr, (std::size_t)(guardValue));
            }
        }

        inline void CheckBack(const void* ptr) override
        {
            const std::uint32_t* guardPtr = reinterpret_cast<const std::uint32_t*>(ptr);
            const std::uint32_t		guardValue = *guardPtr;
            if (guardValue != MAGIC_NUMBER_BACK)
            {
                std::fprintf(stderr, "%s : Back guard damaged for address '%p', was '%zu'\n",
                    getName(), ptr, (std::size_t)(guardValue));
            }
        }
    };
}

#endif //GAME_SIMPLEMEMORYBOUNDCHECKER_H
