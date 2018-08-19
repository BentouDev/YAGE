#ifndef YAGE_GAME_RTTI_LOADER_H
#define YAGE_GAME_RTTI_LOADER_H

#include <Core/GameLib/GameLibApi.h>
#include <Core/RTTI/RTTIStorage.h>

namespace Meta
{
    namespace GameLib
    {
        void RegisterReflection(RTTI::IStorage& storage);
    }
}

#endif // YAGE_GAME_RTTI_LOADER_H