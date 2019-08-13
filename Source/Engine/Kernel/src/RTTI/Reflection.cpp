#include <RTTI/Reflection.h>
#include <RTTI/RTTIRegister.h>
#include <RTTI/ClassResolver.h>
#include <EASTL/hash_map.h>
#include <EASTL/string.h>
#include "RTTI/Register.h"

namespace Meta
{
    struct STypeDefinition
    {
        eastl::string   name;
        RTTI::TypeInfo* info{ nullptr };
    };

    namespace detail
    {
        using TTypeDefinitons = eastl::hash_map<eastl::string, STypeDefinition*>;

        TTypeDefinitons* _definedTypes{ nullptr };
    }

    detail::TTypeDefinitons& getCache()
    {
        using namespace detail;

        if (!_definedTypes)
        {
            _definedTypes = new TTypeDefinitons;

            RTTI::detail::CallOnRTTIShutdown([]() {
                delete _definedTypes;
            });
        }

        return (*_definedTypes);
    }

    YAGE_API RTTI::TypeInfo* registerOrGetType(const char* name)
    {
        auto itr = getCache().find(name);
        if (itr == getCache().end())
        {
            STypeDefinition* def = YAGE_CREATE_NEW(Memory::GetDefaultBlock<STypeDefinition>(), STypeDefinition)();
            def->name = name;

            getCache()[name] = def;

            return nullptr;
        }
        else
        {
            YAGE_ASSERT((*itr).second->info, 
                "Class {} RTTI declaration was found, but definition is missing!"
                "Probably means missing layer or orphaned class!", name);

            return (*itr).second->info;
        }
    }

    namespace detail
    {
        YAGE_API void InjectTypeImpl(const char* name, RTTI::TypeInfo* info)
        {
            auto itr = getCache().find(name);
            if (itr != getCache().end())
            {
                (*itr).second->info = info;
            }
            else
            {
                YAGE_ASSERT(false, "Attempt to inject RTTI implementation for type {} which was not registered before!", name);
            }
        }
    }
}