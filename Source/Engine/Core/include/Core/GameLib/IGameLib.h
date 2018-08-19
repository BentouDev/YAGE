#ifndef YAGE_IGAMELIB_H
#define YAGE_IGAMELIB_H

#include "GameLibApi.h"

namespace RTTI
{
    class IStorage;
}

namespace Core
{
    class GAME_LIB_API IGameLib
    {
    public:
        virtual ~IGameLib()
        { }

        virtual void RegisterRTTI(RTTI::IStorage& storage) = 0;
    };
}

#endif // !YAGE_IGAMELIB_H
