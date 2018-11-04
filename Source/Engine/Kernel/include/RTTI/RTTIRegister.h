#ifndef YAGE_RTTI_REGISTER_H
#define YAGE_RTTI_REGISTER_H

#include <functional>
#include <Utils/List.h>
#include <Utils/String.h>
#include <Utils/CompileString.h>

namespace Meta
{
    class RegisterClass;
}

namespace RTTI
{
    class TypeInfo;
    class EnumInfo;
    class ClassInfo;
    class FieldInfo;

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

        void LoadLayer  (ILayer* layer);
        void UnloadLayer(ILayer* layer);

    private:
        // Layers can only resolve down, so there's no need to process already registered types after unload of upmost layer
        Utils::List<ILayer*> Layers;
    };
}

namespace Meta
{
    class RegisterClass
    {
        using TResolveFunc = std::function<void(RTTI::Register&, RTTI::ClassInfo&)>;
        using TResolver = std::pair<RTTI::ClassInfo*, TResolveFunc>;

        friend class RTTI::Register;
        RegisterClass() = delete;

        Memory::IMemoryBlock& _memory;

        template <typename T>
        TResolver PredeclareClass(Utils::CompileString& name)
        {
            auto* info = NewClass(name, _memory);

            Declare<T>();

            return
            {
                info, 
                [this](RTTI::Register& reg, RTTI::ClassInfo& info)
                {
                    Define<T>(reg, info);
                } 
            };
        }

    protected:
        RTTI::ClassInfo* NewClass(Utils::CompileString& name, Memory::IMemoryBlock& block);

        // template <typename T, typename TField>
        // RTTI::FieldInfo* RegisterField(Utils::CompileString& field_name, typename T::TField(*offset))
        // {
        //     RTTI::TypeInfo*  type  = GetType<TField>();
        //     RTTI::FieldInfo* field = YAGE_CREATE_NEW(_memory, RTTI::FieldInfo)(field_name, type);
        //     return field;
        // }

        template <typename T>
        void Declare(RTTI::Register& reg, RTTI::ClassInfo& data)
        { }

        template <typename T>
        void Define(RTTI::Register& reg, RTTI::ClassInfo& data)
        { }
    };
}

#endif // !YAGE_RTTI_REGISTER_H
