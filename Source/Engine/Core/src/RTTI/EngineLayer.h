#ifndef YAGE_RTTI_ENGINE_LAYER_H
#define YAGE_RTTI_ENGINE_LAYER_H

#include <RTTI/BaseLayer.h>

class ClassInfo;
class EnumInfo;

namespace Memory
{
    class IMemoryBlock;
}

namespace RTTI
{
    class EngineLayer : public BaseLayer
    {
    public:
        explicit EngineLayer(){}
        virtual ~EngineLayer(){}

    protected:
        // ILayer
        virtual bool OnLoad() override;
    };
}

#endif // !YAGE_RTTI_ENGINE_LAYER_H
