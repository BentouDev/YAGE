#ifndef YAGE_RTTI_ENGINE_LOADER_H
#define YAGE_RTTI_ENGINE_LOADER_H

#include <RTTI/RTTIStorage.h>

namespace Meta
{
    namespace Engine
    {
        void RegisterClasses(RTTI::IStorage& storage);
    }
}

#endif // YAGE_RTTI_ENGINE_LOADER_H