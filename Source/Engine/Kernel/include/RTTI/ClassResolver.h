#ifndef CLASS_RESOLVER_H
#define CLASS_RESOLVER_H

#include <functional>
#include <utility>
#include <RTTI/ClassInfo.h>

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
    void RegisterType(Utils::CompileString& name, RTTI::IRegister& rtti);
}

namespace Meta
{
    namespace detail
    {
        using TResolveFunc = std::function<void(RTTI::IRegister&, RTTI::ClassInfo&)>;
        using TResolver    = std::pair<RTTI::ClassInfo*, TResolveFunc>;
    }

    class ClassResolver
    {
        template <typename T>
        friend void RTTI::RegisterType(Utils::CompileString& name, RTTI::IRegister& rtti);
        friend class RTTI::Register;

        ClassResolver() = default;

        template <typename T>
        detail::TResolver PredeclareClass(Utils::CompileString& name, RTTI::IRegister& reg)
        {
            auto* info = ClassStorage<T>::GetClassInfo();
            new (info) RTTI::ClassInfo(name, reg.GetMemory());

            Declare<T>(*(reinterpret_cast<RTTI::Register*>(&reg)), *info);

            return
            {
                info,
                [this](RTTI::IRegister& reg, RTTI::ClassInfo& info)
                {
                    Define<T>(*(reinterpret_cast<RTTI::Register*>(&reg)), info);
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
        { }

        template <typename T>
        void Define(RTTI::Register& reg, RTTI::ClassInfo& data)
        { }
    };
}

#endif // CLASS_RESOLVER_H