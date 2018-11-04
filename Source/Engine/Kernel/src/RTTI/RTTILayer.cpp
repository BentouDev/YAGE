#include "RTTI/RTTILayer.h"

namespace RTTI
{
    bool ILayer::Load(ClassResolvePolicy policy)
    {
        // pass container for some sort of intermediate representation
        OnLoad();

        // after all types are registered, resolve them

        // if all types were resolved successfully, register them

        _loaded = true;
    }

    bool ILayer::Unload()
    {

    }

    bool ILayer::IsLoaded() const
    {
        return _loaded;
    }
}