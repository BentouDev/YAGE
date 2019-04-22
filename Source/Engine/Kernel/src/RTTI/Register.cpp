#include "Register.h"
#include "RTTI/RTTILayer.h"

namespace RTTI
{
	namespace detail
	{
		IRegister* _registerInstance{ nullptr };
	}

    void SetupRTTI(Memory::IMemoryBlock& memory)
    {
        if (!detail::_registerInstance)
        {
            auto* _instance = new Register(memory);
            detail::_registerInstance = _instance;
        }
    }

    void ShutdownRTTI()
    {
        if (detail::_registerInstance)
        {
            delete detail::_registerInstance;
            detail::_registerInstance = nullptr;
        }
    }

	IRegister* GetRegister()
	{
		YAGE_ASSERT(detail::_registerInstance, "RTTI : Register accessed before Start or after Shutdown!");
		return detail::_registerInstance;
	}

    Register::Register(Memory::IMemoryBlock& memory)
        : _memory(memory), Layers(/*memory*/) // #NewAlloc
    { }

    ClassInfo* Register::GetClass(const char* name)
    {
        auto* clazz = FindClass(name);
        YAGE_ASSERT(clazz, "RTTI : Unresolved class definition '{}'!", name);

        return clazz;
    }
    
    EnumInfo* Register::GetEnum(const char* name)
    {
        auto* enumz = FindEnum(name);
        YAGE_ASSERT(enumz, "RTTI : Unresolved enum definition '{}'!", name);

        return enumz;
    }

    TypeInfo* Register::GetType(const char* name)
    {
        auto* type = FindType(name);
        YAGE_ASSERT(type, "RTTI : Unresolved type definition '{}'!", name);

        return type;
    }

    EnumInfo* Register::FindEnum([[maybe_unused]]const char* name)
    {
        // TODO: implement search through layers
        return nullptr;
    }

    TypeInfo* Register::FindType([[maybe_unused]]const char* name)
    {
        // TODO: implement search through layers
        return nullptr;
    }

    ClassInfo* Register::FindClass([[maybe_unused]]const char* name)
    {
        // TODO: implement search through layers
        return nullptr;
    }

    void Register::ResolveClass(Meta::detail::TResolver&& resolver)
    {
        _currentResolvers->push_back(std::move(resolver));
    }

    void Register::DefineClasses()
    {
        for (auto itr : (*_currentResolvers))
        {
            itr.second(*this, *itr.first);
        }
    }

    void Register::LoadLayer(ILayer* layer)
    {
        if (auto itr = std::find(Layers.begin(), Layers.end(), layer); itr != Layers.end())
        {
            YAGE_ASSERT(false, "RTTI : Attempt to register same layer twice!");
            return;
        }

		_currentResolvers = std::make_unique<TLayerCache>();// Memory::GetDefaultBlock<Register>());

        if (layer->Load(ClassResolvePolicy::Strict))
        {
            DefineClasses();
            Layers.push_back(layer);
        }
        else
        {
            YAGE_ASSERT(false, "RTTI : Failed attempt to register layer!");
        }

        _currentResolvers.reset();
    }

    void Register::UnloadLayer(ILayer* layer)
    {
        if (layer == Layers.back())
        {
            layer->Unload();

            Layers.erase(Layers.begin() + Layers.size() - 1);
        }
        else
        {
            YAGE_ASSERT(false, "RTTI : Attempt to unload layer out of order!");
        }
    }
}