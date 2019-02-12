#ifndef BASE_LAYER_H
#define BASE_LAYER_H

#include <RTTI/RTTILayer.h>
#include <RTTI/RTTIStorage.h>
#include <Utils/List.h>

namespace RTTI
{
    class BaseLayer : public ILayer, public IStorage
    {
    protected:
        explicit BaseLayer();
        virtual ~BaseLayer();

        // ILayer
        virtual bool OnUnload() override;

        // IStorage
        virtual void RegisterType(ClassInfo* clazz) override;
        virtual void RegisterType(EnumInfo*  enumz) override;

        Utils::List<ClassInfo*> Classes;
        Utils::List<EnumInfo*>  Enums;
    };
}

#endif //BASE_LAYER_H