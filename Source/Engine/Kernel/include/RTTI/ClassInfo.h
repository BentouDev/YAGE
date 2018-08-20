#ifndef YAGE_RTTI_CLASS_INFO_H
#define YAGE_RTTI_CLASS_INFO_H

#include <type_traits>
#include "TypeInfo.h"
#include "FieldInfo.h"
#include "MethodInfo.h"

namespace RTTI
{
    class Register;

    class ClassInfo : public TypeInfo
    {
        friend class Register;

    protected:
        explicit ClassInfo(Utils::CompileString& name, Memory::IMemoryBlock& memory)
            : TypeInfo(name)
            , Interfaces(memory)
            , Methods(memory)
            , Fields(memory)
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
    };
}

#endif // !YAGE_RTTI_CLASS_INFO_H