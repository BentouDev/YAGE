#ifndef YAGE_RTTI_CLASS_INFO_H
#define YAGE_RTTI_CLASS_INFO_H

#include <type_traits>
#include <Utils/Attribute.h>
#include "TypeInfo.h"
#include "FieldInfo.h"
#include "MethodInfo.h"

namespace Meta
{
    class RegisterClass;
}

namespace RTTI
{
    class Register;

    // How to create ClassInfo instance? 
    // either derive a new type
    // or specialize templated method

    class ClassInfo : public TypeInfo
    {
        friend class Meta::RegisterClass;

    protected:
        explicit ClassInfo(Utils::CompileString& name, Memory::IMemoryBlock& memory)
            : TypeInfo(name)
            , Interfaces(memory)
            , Methods(memory)
            , Fields(memory)
            , Attributes(memory)
            , Super(nullptr)
        { }

        virtual bool Resolve(Register& ) = 0;

    public:
        const ClassInfo* GetSuper() const { return Super; }

    protected:
        ClassInfo*    Super;
        Utils::String CanonicalName;

        Utils::List<ClassInfo*> Interfaces;
        Utils::List<MethodInfo> Methods;
        Utils::List<FieldInfo>  Fields;
        Utils::List<Attribute>  Attributes;
    };
}

#endif // !YAGE_RTTI_CLASS_INFO_H