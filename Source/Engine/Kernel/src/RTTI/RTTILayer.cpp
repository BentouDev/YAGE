#include "RTTI/RTTILayer.h"

namespace RTTI
{
    bool ILayer::Load(RTTI::IRegister& rtti, ClassResolvePolicy policy)
    {
        if (_loaded)
        {
            return false;
        }

        // pass container for some sort of intermediate representation
        bool result = OnLoad(rtti);

        // after all types are registered, resolve them

        // if all types were resolved successfully, register them

        if (result)
        {
            _loaded = true;
            return true;
        }

        // Failed, unload

        return false;
    }

    bool ILayer::Unload()
    {
        if (!_loaded)
        {
            return false;
        }

        _loaded = false;

        return OnUnload();
    }

    bool ILayer::IsLoaded() const
    {
        return _loaded;
    }
}