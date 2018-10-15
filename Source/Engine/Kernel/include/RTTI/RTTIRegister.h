#ifndef YAGE_RTTI_REGISTER_H
#define YAGE_RTTI_REGISTER_H

#include <Utils/List.h>
#include <Utils/String.h>

namespace Meta
{
    class RegisterClass;
}

namespace RTTI
{
    class TypeInfo;
    class EnumInfo;
    class ClassInfo;

    class ILayer;

    class IRegister
    {
        friend class RegisterClass;

    protected:
        virtual TypeInfo*  GetType (const Utils::String& name) = 0;
        virtual EnumInfo*  GetEnum (const Utils::String& name) = 0;
        virtual ClassInfo* GetClass(const Utils::String& name) = 0;
    };

    class Register : public IRegister
    {
    public:
        TypeInfo*  FindType (const Utils::String& name);
        EnumInfo*  FindEnum (const Utils::String& name);
        ClassInfo* FindClass(const Utils::String& name);

    // IRegister
    protected:
        virtual TypeInfo*  GetType (const Utils::String& name) override;
        virtual EnumInfo*  GetEnum (const Utils::String& name) override;
        virtual ClassInfo* GetClass(const Utils::String& name) override;

    private:
        // Layers can only resolve down, so there's no need to process already registered types after unload of upmost layer
        Utils::List<ILayer*> _Layers;
    };
}

namespace Meta
{
    class RegisterClass
    {
        friend class RTTI::Register;
        RegisterClass() = delete;

    protected:
        template <typename T>
        void Declare(RTTI::Register& reg, RTTI::ClassInfo& data)
        { }

        template <typename T>
        void Define(RTTI::Register& reg, RTTI::ClassInfo& data)
        { }
    };
}

#endif // !YAGE_RTTI_REGISTER_H
