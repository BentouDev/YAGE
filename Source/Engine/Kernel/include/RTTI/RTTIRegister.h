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
        TypeInfo*  FindType (const Utils::String& name);
        EnumInfo*  FindEnum (const Utils::String& name);
        ClassInfo* FindClass(const Utils::String& name);

    private:
        // Layers can only resolve down, so there's no need to process already registered types after unload of upmost layer
        Utils::List<ILayer*> _Layers;
    };
}

namespace Meta
{
    class RegisterClass
    {
        friend class Register;
        RegisterClass() = delete;

    protected:
        template <typename T>
        void Declare(Register& reg, RTTI::ClassInfo& data)
        { }

        template <typename T>
        void Define(Register& reg, RTTI::ClassInfo& data)
        { }
    };
}

#endif // !YAGE_RTTI_REGISTER_H
