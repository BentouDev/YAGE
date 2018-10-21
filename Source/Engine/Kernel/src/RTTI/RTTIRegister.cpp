#include "RTTI/RTTIRegister.h"
#include "RTTI/RTTILayer.h"

namespace RTTI
{
    ClassInfo* Register::GetClass(const Utils::String& name)
    {
        auto* clazz = FindClass(name);
        YAGE_ASSERT(clazz, "RTTI : Unresolved class definition '{}'!", name.c_str());

        return clazz;
    }

    void Register::LoadLayer(ILayer* layer)
    {
        if (auto itr = std::find(Layers.begin(), Layers.end(), layer); itr != Layers.end())
        {
            YAGE_ASSERT(false, "RTTI : Attempt to register same layer twice!");
            return;
        }

        layer->Load();

        Layers.add(layer);
    }

    void Register::UnloadLayer(ILayer* layer)
    {
        if (layer == Layers.back())
        {
            layer->Unload();

            Layers.eraseAddress(&layer);
        }
        else
        {
            YAGE_ASSERT(false, "RTTI : Attempt to unload layer out of order!");
        }
    }
}