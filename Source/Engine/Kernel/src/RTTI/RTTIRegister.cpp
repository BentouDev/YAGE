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

        if (layer->Load(ClassResolvePolicy::Strict))
        {
            Layers.add(layer);
        }

        YAGE_ASSERT(false, "RTTI : Failed attempt to register layer!");
    }

    void Register::UnloadLayer(ILayer* layer)
    {
        if (layer == Layers.back())
        {
            layer->Unload();

            Layers.eraseAt(Layers.size() - 1);
        }
        else
        {
            YAGE_ASSERT(false, "RTTI : Attempt to unload layer out of order!");
        }
    }
}