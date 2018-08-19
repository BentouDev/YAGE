#ifndef YAGE_RTTI_ENGINE_LAYER_H
#define YAGE_RTTI_ENGINE_LAYER_H

#include "Core/RTTI/RTTILayer.h"
#include "Core/RTTI/RTTIStorage.h"
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
        virtual bool Load() override;
        virtual bool Unload() override;
        virtual bool IsLoaded() const override;

        // IStorage
        virtual void RegisterType(ClassInfo* clazz) override;
        virtual void RegisterType(EnumInfo*  enumz) override;

    private:
        bool _loaded;
        Utils::List<ClassInfo*> Classes;
        Utils::List<EnumInfo*>  Enums;
    };
}

#endif // !YAGE_RTTI_ENGINE_LAYER_H
