#ifndef YAGE_REFLECTION_H
#define YAGE_REFLECTION_H

#include <type_traits>

namespace Utils
{
    class String;
}

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
            static_assert(std::is_integral<T>::value, "Cannot call GetIntegral() for non-integral types!");
            return nullptr;
        }

        template <typename T>
        RTTI::ClassInfo* GetClass()
        {
            static_assert(std::is_class<T>::value, "Cannot call GetClass() for non-class types!");
            return nullptr;
        }
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
}

#endif // !YAGE_REFLECTION_H
