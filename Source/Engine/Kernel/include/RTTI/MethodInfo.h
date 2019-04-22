#ifndef YAGE_RTTI_METHOD_INFO_H
#define YAGE_RTTI_METHOD_INFO_H

#include "Defines.h"

namespace RTTI
{
    class MethodInfo : public MetaInfo
    {
        friend class Register;

    protected:
        MethodInfo(Utils::CompileString& name, Memory::IMemoryBlock& memory)
            : MetaInfo(name), ReturnType(nullptr), Parameters()//memory)// #NewAlloc
        { }

        void Resolve();

        TypeInfo*              ReturnType;
        Utils::List<TypeInfo*> Parameters;
    };
}

#endif // !YAGE_RTTI_METHOD_INFO_H