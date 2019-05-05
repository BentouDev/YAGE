#include <RTTI/Reflection.h>
#include <RTTI/RTTIRegister.h>
#include <RTTI/ClassResolver.h>
#include <EASTL/hash_map.h>
#include <EASTL/string.h>

namespace Meta
{
    struct STypeDefinition
    {
        eastl::string   name;
        RTTI::TypeInfo* info{ nullptr };
    };

    eastl::hash_map<eastl::string, STypeDefinition*> _definedTypes;

    YAGE_API RTTI::TypeInfo* registerOrGetType(const char* name)
    {
        auto itr = _definedTypes.find(name);
        if (itr == _definedTypes.end())
        {
            STypeDefinition* def = YAGE_CREATE_NEW(Memory::GetDefaultBlock<STypeDefinition>(), STypeDefinition)();
            def->name = name;

            _definedTypes[name] = def;

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
            auto itr = _definedTypes.find(name);
            if (itr != _definedTypes.end())
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