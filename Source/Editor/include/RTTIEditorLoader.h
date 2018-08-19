#ifndef YAGE_EDITOR_RTTI_LOADER_H
#define YAGE_EDITOR_RTTI_LOADER_H

#include <Core/RTTI/RTTIStorage.h>

namespace Meta
{
    namespace Editor
    {
        void RegisterClasses(RTTI::IStorage& storage);
    }
}

#endif // !YAGE_EDITOR_RTTI_LOADER_H