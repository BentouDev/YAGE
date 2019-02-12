#ifndef REGISTER_H_
#define REGISTER_H_

#include <RTTI/ClassResolver.h>
#include <RTTI/RTTIRegister.h>

namespace RTTI
{
    class YAGE_API Register : public IRegister
    {
    public:
        TypeInfo*  FindType (const char* name);
        EnumInfo*  FindEnum (const char* name);
        ClassInfo* FindClass(const char* name);

        explicit Register(Memory::IMemoryBlock& memory);

    // IRegister
        virtual void ResolveClass(Meta::detail::TResolver&& resolver) override;
        virtual Memory::IMemoryBlock& GetMemory() override { return _memory; }
        virtual Meta::ClassResolver& GetClassResolver() override { return _resolver; }

    protected:
        virtual TypeInfo*  GetType (const char* name) override;
        virtual EnumInfo*  GetEnum (const char* name) override;
        virtual ClassInfo* GetClass(const char* name) override;

        virtual void LoadLayer  (ILayer* layer) override;
        virtual void UnloadLayer(ILayer* layer) override;

    private:
        void DefineClasses();

        using TLayerCache = Utils::List<Meta::detail::TResolver>;

        Memory::IMemoryBlock& _memory;
        Meta::ClassResolver   _resolver;

        // Layers can only resolve down, so there's no need to process already registered types after unload of upmost layer
        Utils::List<ILayer*>         Layers;
        std::unique_ptr<TLayerCache> _currentResolvers;
    };
}

#endif // REGISTER_H_