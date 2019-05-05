#include "Register.h"
#include "RTTI/RTTILayer.h"
#include "RTTI/BaseLayer.h"
#include "RTTI/RTTILayerDefinition.h"
#include "EASTL/hash_map.h"
#include "EASTL/string.h"
#include "Utils/DefaultBlock.h"

namespace kernel::rtti::detail
{
    namespace impl
    {
        using TLayerCache = eastl::hash_map<size_t, define_layer*>;

        TLayerCache& getCache()
        {
            static TLayerCache _known_subsystems{};
            return _known_subsystems;
        }
    }

    define_layer::define_layer(Utils::CompileString name, const std::type_info& info, TCreator* create, TDeleter* del, TLoader* load)
        : _create(create), _delete(del), _load(load)
    {
        auto index = info.hash_code();
        bool already_exists = impl::getCache().find(index) != impl::getCache().end();

        YAGE_ASSERT(!already_exists, "Attempt to register duplicate of RTTI layer {}!", name.c_str());

        if (!already_exists)
        {
            impl::getCache()[index] = this;
        }
    }

    define_layer::~define_layer()
    {
        auto itr = std::find_if(impl::getCache().begin(), impl::getCache().end(), [&](const auto& entry) {
            return entry.second == this;
        });

        YAGE_ASSERT(itr != impl::getCache().end(), "Destructor of RTTI layer definiton not present in cache!");

        impl::getCache().erase(itr);
    }
}

namespace RTTI
{
    namespace detail
    {
        IRegister* _registerInstance{ nullptr };
    }

    void InitializeLoader(BaseLayer* layer, TLoader* loader)
    {
        layer->_loader = loader;
    }

    IRegister::~IRegister()
    {
        YAGE_ASSERT(detail::_registerInstance == nullptr, "RTTI : IRegister destroyed without RTTI::Shutdown!");
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
            IRegister* tmp = detail::_registerInstance;
            detail::_registerInstance = nullptr;
            delete tmp;
        }
    }

    IRegister& GetRegister()
    {
        YAGE_ASSERT(detail::_registerInstance, "RTTI : Register accessed before Start or after Shutdown!");
        return *detail::_registerInstance;
    }

    Register::Register(Memory::IMemoryBlock& memory)
        : _memory(memory), Layers(/*memory*/) // #NewAlloc
    { }

    TypeInfo* Register::GetType(const std::type_info& info)
    {
        TypeInfo* result = nullptr;

        for (auto itr = Layers.rbegin(); itr != Layers.rend(); itr++)
        {
            if ((*itr)->IsLoaded() && (*itr)->TryGetType(info, result))
            {
                break;
            }
        }

        return result;
    }

    EnumInfo* Register::GetEnum(const std::type_info& info)
    {
        EnumInfo* result = nullptr;
        
        for (auto itr = Layers.rbegin(); itr != Layers.rend(); itr++)
        {
            if ((*itr)->IsLoaded() && (*itr)->TryGetEnum(info, result))
            {
                break;
            }
        }

        return result;
    }

    ClassInfo* Register::GetClass(const std::type_info& info)
    {
        ClassInfo* result = nullptr;

        for (auto itr = Layers.rbegin(); itr != Layers.rend(); itr++)
        {
            if ((*itr)->IsLoaded() && (*itr)->TryGetClass(info, result))
            {
                break;
            }
        }

        return result;
    }

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

    bool Register::PushLayerInternal(const std::type_info& info)
    {
        auto index = info.hash_code();
        bool found = false;

        for (auto* layer : Layers)
        {
            found |= index == typeid(layer).hash_code();
            if (found) 
                break;
        }

        if (found)
        {
            YAGE_ASSERT(false, "RTTI : Attempt to register same layer twice!");
            return false;
        }

        auto itr = kernel::rtti::detail::impl::getCache().find(index);
        if (itr == kernel::rtti::detail::impl::getCache().end())
        {
            YAGE_ASSERT(false, "RTTI : Attempt to register unknown layer!\nCompiler generated name: '{}'", info.name());
            return false;
        }

        auto* instance = (*itr).second->_create(&Memory::GetDefaultBlock<Register>());
        InitializeLoader(static_cast<BaseLayer*>(instance), (*itr).second->_load);
        LoadLayer(instance);
    }

    bool Register::PopLayerInternal(const std::type_info& info)
    {
        ILayer* layer_ptr = nullptr;
        auto index = info.hash_code();

        for (auto* layer : Layers)
        {
            if (index == typeid(layer).hash_code())
            {
                layer_ptr = layer;
                break;
            }
        }

        if (!layer_ptr)
        {
            YAGE_ASSERT(false, "RTTI : Attempt to release unknown layer!\nCompiler generated name: '{}'", info.name());
            return false;
        }

        UnloadLayer(layer_ptr);

        auto itr = kernel::rtti::detail::impl::getCache().find(index);
        if (itr != kernel::rtti::detail::impl::getCache().end())
        {
            (*itr).second->_delete(&Memory::GetDefaultBlock<Register>(), layer_ptr);
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

        if (layer->Load(*this, ClassResolvePolicy::Strict))
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

namespace Meta
{
    TTypeId lastUniqueId = 0;

    TTypeId ClassResolver::getUniqueId()
    {
        return ++lastUniqueId;
    }
}
