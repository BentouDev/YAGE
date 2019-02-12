#ifndef YAGE_RTTI_ENUM_INFO_H
#define YAGE_RTTI_ENUM_INFO_H

#include "List.h"
#include "Defines.h"
#include "TypeInfo.h"
#include "CompileString.h"

namespace RTTI
{
    class EnumValueInfo : public MetaInfo
    {
    protected:
        EnumValueInfo(Utils::CompileString& name)
            : MetaInfo(name)
        { }
    };

    class EnumInfo : public TypeInfo
    {
    protected:
        EnumInfo(Utils::CompileString& name, Memory::IMemoryBlock& memory)
            : TypeInfo(name)
            , Super(nullptr)
            , Values(memory)
        { }

        TypeInfo*                  Super;
        Utils::List<EnumValueInfo> Values;
    };
}

#endif // YAGE_RTTI_ENUM_INFO_H