#ifndef YAGE_EDITOR_RTTI_LOADER_H
#define YAGE_EDITOR_RTTI_LOADER_H

#include <RTTI/RTTIRegister.h>

namespace Meta
{
    namespace Editor
    {
        void RegisterClasses(RTTI::IRegister& storage);
    }
}

#endif // !YAGE_EDITOR_RTTI_LOADER_H