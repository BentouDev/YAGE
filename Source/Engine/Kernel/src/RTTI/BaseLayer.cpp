#include <RTTI/BaseLayer.h>
#include <Utils/MemoryBlock.h>
#include <Utils/DefaultBlock.h>

namespace RTTI
{
    BaseLayer::BaseLayer() // #NewAlloc
        : Classes()//Memory::GetDefaultBlock<BaseLayer>())
        , Enums()//Memory::GetDefaultBlock<BaseLayer>())
    { }

    BaseLayer::~BaseLayer()
    {
        Unload();
    }

    bool BaseLayer::OnUnload()
    {
        Classes.clear();
        Enums.clear();

        return true;
    }

    void BaseLayer::RegisterType(ClassInfo* clazz)
    {
        if (_loaded)
            return;

        Classes.push_back(clazz);
    }

    void BaseLayer::RegisterType(EnumInfo* enumz)
    {
        if (_loaded)
            return;

        Enums.push_back(enumz);
    }
}