#ifndef CLASS_RESOLVER_H
#define CLASS_RESOLVER_H

#include <RTTI/ClassInfo.h>
#include <RTTI/RTTICommon.h>
#include <RTTI/RTTIRegister.h>
#include <Utils/MacroDefines.h>

namespace Memory
{
    class IMemoryBlock;
}

namespace Utils
{
    class CompileString;
}

namespace RTTI
{
    class IRegister;
    class Register;
    class ClassInfo;

    template <typename T>
    void RegisterType(Utils::CompileString name, RTTI::IRegister& rtti);
}

namespace Meta
{
    namespace detail
    {
        YAGE_API void InjectTypeImpl(const char* name, RTTI::TypeInfo*);
    }

    template <typename T>
    class ClassStorage
    {
        friend class ClassResolver;

        inline static char _clazzMem[sizeof(RTTI::ClassInfo)] = { 0 };

        static RTTI::ClassInfo* GetClassInfo()
        {
            return reinterpret_cast<RTTI::ClassInfo*>(_clazzMem);
        }
    };

    using TTypeId = unsigned long long;

    class YAGE_API ClassResolver
    {
        static TTypeId getUniqueId();

        template <typename T>
        friend void RTTI::RegisterType(Utils::CompileString name, RTTI::IRegister& rtti);
        friend class RTTI::Register;

        ClassResolver() = default;

        template <typename T>
        detail::TResolver PredeclareClass(Utils::CompileString& name, RTTI::IRegister& reg)
        {
            auto* info = ClassStorage<T>::GetClassInfo();
            new (info) RTTI::ClassInfo(name, reg.GetMemory());

            info->Id = getUniqueId();

            Declare<T>(*(reinterpret_cast<RTTI::Register*>(&reg)), *info);

            return
            {
                info,
                [this](RTTI::IRegister& reg, RTTI::ClassInfo& info)
                {
                    Define<T>(*(reinterpret_cast<RTTI::Register*>(&reg)), info);
                    ::Meta::detail::InjectTypeImpl(info.CanonicalName.c_str(), &info);
                }
            };
        }

    protected:
        // template <typename T, typename TField>
        // RTTI::FieldInfo* RegisterField(Utils::CompileString& field_name, typename T::TField(*offset))
        // {
        //     RTTI::TypeInfo*  type  = GetType<TField>();
        //     RTTI::FieldInfo* field = YAGE_CREATE_NEW(_memory, RTTI::FieldInfo)(field_name, type);
        //     return field;
        // }

        template <typename T>
        void Declare(RTTI::Register& reg, RTTI::ClassInfo& data)
        {
            static_assert(false, "Attempt to call ClassResolver::Declare<T>() for unregistered type!");
        }

        template <typename T>
        void Define(RTTI::Register& reg, RTTI::ClassInfo& data)
        {
            static_assert(false, "Attempt to call ClassResolver::Define<T>() for unregistered type!");
        }
    };
}

namespace RTTI
{
    template <typename T>
    void RegisterType(Utils::CompileString name, IRegister& rtti)
    {
        using TBase = typename std::remove_reference<T>::type;
        if constexpr (std::is_integral<TBase>::value)
        {
            rtti.ResolveIntegral(std::move(rtti.CreateIntegralResolver<T>(name, rtti)));
        }
        else if constexpr (std::is_class<TBase>::value)
        {
            rtti.ResolveClass(std::move(rtti.GetClassResolver().PredeclareClass<T>(name, rtti)));
        }
    }
}

#endif // CLASS_RESOLVER_H