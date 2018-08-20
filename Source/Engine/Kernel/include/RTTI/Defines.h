#ifndef YAGE_RTTI_DEFINES_H
#define YAGE_RTTI_DEFINES_H

#include <Utils/String.h>
#include <Utils/CompileString.h>

typedef uint16_t type_t;

namespace RTTI
{
    namespace ValueType
    {
        enum TYPE
        {
            Value,
            Pointer,
            Reference
        };
    }

    namespace AccessType
    {
        enum TYPE
        {
            None = 0,
            Readable = 1,
            Writable = 2,
            Mutable = Readable | Writable,
            Const = Readable
        };
    }

    class MetaInfo
    {
    protected:
        explicit MetaInfo(Utils::CompileString& name)
            : Name(name.c_str())
        { }

    public:
        virtual ~MetaInfo() = default;

        const Utils::String Name;
    };
}

#endif // !YAGE_RTTI_DEFINES_H