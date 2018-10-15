#include "RTTI/RTTIRegister.h"

namespace RTTI
{
    ClassInfo* Register::GetClass(const Utils::String& name)
    {
        auto* clazz = FindClass(name);
        YAGE_ASSERT(clazz, "RTTI : Unresolved class definition '{}'!", name.c_str());

        return clazz;
    }
}