#ifndef YAGE_RTTI_LAYER_H
#define YAGE_RTTI_LAYER_H

#include <Utils/Defines.h>

namespace RTTI
{
    enum class ClassResolvePolicy
    {
        Strict,
        Permissive
    };

    // Derived by engine layer, game lib layer and game script layer
    class YAGE_API ILayer
    {
    protected:
        explicit ILayer()
        { }

    public:
        virtual ~ILayer()
        { }

        bool Load(ClassResolvePolicy policy);
        bool Unload();
        bool IsLoaded() const;

    protected:
        virtual bool OnLoad() = 0;
        virtual bool OnUnload() = 0;

        bool _loaded;
    };
}

#endif // !YAGE_RTTI_LAYER_H