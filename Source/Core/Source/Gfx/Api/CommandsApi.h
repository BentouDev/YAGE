//
// Created by bentoo on 7/5/16.
//

#ifndef GAME_COMMANDSAPI_H_H
#define GAME_COMMANDSAPI_H_H

#include <cstdint>
#include <Utils/include/TypeInfo.h>
#include <vector>

namespace Gfx
{
    struct CommandInfo
    {
        type_t type;
        uint32_t size;

        CommandInfo(type_t t, uint32_t s) : type(t), size(s) {}

    };

    class CommandsApi
    {
    public:

        template <typename T>
        auto registerCommand() -> void
        {
            internalRegisterCommand<T, sizeof(T)>();
        }

    protected:
        std::vector<CommandInfo> commandTypes;

        template <typename T, uint32_t size>
        auto internalRegisterCommand() -> void
        {
            commandTypes.emplace_back(TypeInfo<T>::id(), size);
        }
    };
}

#endif //GAME_COMMANDSAPI_H_H
