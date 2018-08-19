#ifndef YAGE_RTTI_LAYER_H
#define YAGE_RTTI_LAYER_H

namespace RTTI
{
    // Derived by engine layer, game lib layer and game script layer
    class ILayer
    {
    protected:
        explicit ILayer()
        { }

    public:
        virtual ~ILayer()
        { }

        virtual bool Load() = 0;
        virtual bool Unload() = 0;
        virtual bool IsLoaded() const = 0;
    };
}

#endif // !YAGE_RTTI_LAYER_H
