#ifndef YAGE_RTTI_REGISTER_H
#define YAGE_RTTI_REGISTER_H

#include <Utils/List.h>
#include <Utils/String.h>
#include <Reflection/ReflectionInfo.h>

namespace RTTI
{
    class ILayer;

    class Register
    {
    public:
        ClassInfo* FindClass(const Utils::String& name);

    private:
        Utils::List<ILayer*> _Layers;
    };
}

#endif // !YAGE_RTTI_REGISTER_H
