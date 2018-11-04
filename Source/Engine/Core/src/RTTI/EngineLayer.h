#ifndef YAGE_RTTI_ENGINE_LAYER_H
#define YAGE_RTTI_ENGINE_LAYER_H

#include <RTTI/RTTILayer.h>
#include <RTTI/RTTIStorage.h>
#include <Utils/List.h>

class ClassInfo;
class EnumInfo;

namespace Memory
{
    class IMemoryBlock;
}

namespace RTTI
{
    class EngineLayer : public ILayer, public IStorage
    {
    public:
        explicit EngineLayer(Memory::IMemoryBlock& memory);
        virtual ~EngineLayer();

        // ILoader
        virtual bool OnLoad() override;
        virtual bool OnUnload() override;

        // IStorage
        virtual void RegisterType(ClassInfo* clazz) override;
        virtual void RegisterType(EnumInfo*  enumz) override;

    private:
        Utils::List<ClassInfo*> Classes;
        Utils::List<EnumInfo*>  Enums;
    };
}

#endif // !YAGE_RTTI_ENGINE_LAYER_H
