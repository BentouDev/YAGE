#ifndef YAGE_RTTI_COMMON_H
#define YAGE_RTTI_COMMON_H

#include <utility>
#include <functional>

namespace RTTI
{
    class IRegister;
    class Register;
    class ClassInfo;
}

namespace Meta
{
    namespace detail
    {
        using TResolveFunc = std::function<void(RTTI::IRegister&, RTTI::ClassInfo&)>;
        using TResolver    = std::pair<RTTI::ClassInfo*, TResolveFunc>;
    }
}

#endif // YAGE_RTTI_COMMON_H