#ifndef YAGE_RTTI_ENUM_INFO_H
#define YAGE_RTTI_ENUM_INFO_H

#include <Utils/List.h>
#include <Utils/Defines.h>
#include <RTTI/TypeInfo.h>
#include <Utils/CompileString.h>

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
            , Values() // #newalloc memory)
        { }

        TypeInfo*                  Super;
        Utils::List<EnumValueInfo> Values;
    };
}

#endif // YAGE_RTTI_ENUM_INFO_H