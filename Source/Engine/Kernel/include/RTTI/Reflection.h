#ifndef YAGE_REFLECTION_H
#define YAGE_REFLECTION_H

#include <type_traits>
#include <Utils/String.h>
#include <Utils/Defines.h>

namespace RTTI
{
    class TypeInfo;
    class EnumInfo;
    class ClassInfo;
}

namespace Meta
{
    namespace detail
    {
        template <typename T>
        RTTI::TypeInfo* GetIntegral()
        {
            static_assert(false, "Attempt to call GetIntegral() for unregistered type!");
            return nullptr;
        }

        template <typename T>
        RTTI::ClassInfo* GetClass()
        {
            static_assert(false, "Attempt to call GetClass() for unregistered type!");
            return nullptr;
        }

        template <typename T>
        struct define_rtti
        { };
    }

    RTTI::TypeInfo*  FindType (const Utils::String& name);
    RTTI::EnumInfo*  FindEnum (const Utils::String& name);
    RTTI::ClassInfo* FindClass(const Utils::String& name);

    template <typename T>
    auto GetType()
    {
        using TBase = typename std::remove_reference<T>::type;

        if constexpr (std::is_integral<TBase>::value)
        {
            return detail::GetIntegral<TBase>();
        }
        else if constexpr (std::is_class<TBase>::value)
        {
            return detail::GetClass<TBase>();
        }
        else
        {
            return nullptr;
        }
    }

    YAGE_API RTTI::TypeInfo* registerOrGetType(const char* name);
}

#define YAGE_DECLARE_CLASS_RTTI(clazz) \
        template <> inline RTTI::ClassInfo* ::Meta::detail::GetClass<typename clazz>() \
        { return static_cast<RTTI::ClassInfo*>(::Meta::registerOrGetType(#clazz)); }

#define YAGE_DEFINE_CLASS_RTTI_IMPL(clazz, name) \
        template<> \
        struct ::Meta::detail::define_rtti<clazz> { \
            define_rtti() { ::Meta::detail::GetClass<clazz>(); } \
        }; \
        static const ::Meta::detail::define_rtti<clazz> name{}

#define YAGE_DEFINE_CLASS_RTTI(clazz) YAGE_DEFINE_CLASS_RTTI_IMPL( clazz, YAGE_MAKE_UNIQUE( rtti_def_ ) )

#endif // !YAGE_REFLECTION_H