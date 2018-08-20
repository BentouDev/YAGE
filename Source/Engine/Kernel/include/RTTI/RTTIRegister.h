#ifndef YAGE_RTTI_REGISTER_H
#define YAGE_RTTI_REGISTER_H

#include <Utils/List.h>
#include <Utils/String.h>

namespace RTTI
{
    class TypeInfo;
    class EnumInfo;
    class ClassInfo;

    class ILayer;

    class Register
    {
    public:
        TypeInfo*  FindType(const Utils::String& name);
        EnumInfo*  FindEnum(const Utils::String& name);
        ClassInfo* FindClass(const Utils::String& name);

    private:
        Utils::List<ILayer*> _Layers;
    };
}

#endif // !YAGE_RTTI_REGISTER_H
