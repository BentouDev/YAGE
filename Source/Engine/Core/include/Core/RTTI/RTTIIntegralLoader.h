#ifndef YAGE_RTTI_INTEGRAL_LOADER_H
#define YAGE_RTTI_INTEGRAL_LOADER_H

#include <RTTI/RTTIRegister.h>
#include <RTTI/BaseLayer.h>
#include <RTTI/RTTILayerDefinition.h>
#include <RTTI/ClassResolver.h>
#include <RTTI/Reflection.h>

namespace RTTI
{
    class IntegralLayer final : public BaseLayer{ };
}

#define YAGE_DECLARE_INTEGRAL_RTTI(type) \
        template <> inline RTTI::TypeInfo* ::Meta::detail::GetIntegral<typename type>() \
        { return static_cast<RTTI::TypeInfo*>(::Meta::registerOrGetType(#type)); }

YAGE_DECLARE_INTEGRAL_RTTI(int);
YAGE_DECLARE_INTEGRAL_RTTI(float);
YAGE_DECLARE_INTEGRAL_RTTI(double);
YAGE_DECLARE_INTEGRAL_RTTI(bool);
YAGE_DECLARE_INTEGRAL_RTTI(char);
YAGE_DECLARE_INTEGRAL_RTTI(size_t);

#undef YAGE_DECLARE_INTEGRAL_RTTI

#endif // YAGE_RTTI_INTEGRAL_LOADER_H