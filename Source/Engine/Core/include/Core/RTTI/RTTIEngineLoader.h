#ifndef YAGE_RTTI_ENGINE_LOADER_H
#define YAGE_RTTI_ENGINE_LOADER_H

#include <RTTI/RTTIRegister.h>

namespace Meta
{
    namespace Engine
    {
        void RegisterClasses(RTTI::IRegister& storage);
    }
}

#endif // YAGE_RTTI_ENGINE_LOADER_H