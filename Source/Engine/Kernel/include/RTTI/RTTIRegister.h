#ifndef YAGE_RTTI_REGISTER_H
#define YAGE_RTTI_REGISTER_H

#include <functional>
#include <memory>
#include <Utils/Defines.h>
#include <Utils/List.h>
#include <Utils/String.h>
#include <Utils/DefaultBlock.h>
#include <Utils/CompileString.h>
#include <RTTI/RTTICommon.h>

namespace RTTI
{
    class TypeInfo;
    class EnumInfo;
    class ClassInfo;
    class FieldInfo;

    class IRegister;
    class Register;

    class ILayer;
}

namespace Meta
{
    class ClassResolver;
}

namespace RTTI
{
    /*
        During layer loading all types are first predeclared (full type list is built)
        and only then defined (method, filed and base classes pointers are being resolved.

        Kernel provides single static RTTI registry, with stack of layers.
        Layers aggregates it's meta data independently inside.

        RTTI meta data objects are accessed only through appropriate smart pointer,
        so risk of accessing data of unloaded layer is neglected.

        Templated type query is resolved in compile time.
        String-based type query is resolved top-down through layers.
    */

    class YAGE_API IRegister
    {
        template <typename T>
        friend void RegisterType(Utils::CompileString& name, IRegister& rtti);
        friend class ClassResolver;

    protected:
        virtual TypeInfo*  GetType (const char* name) = 0;
        virtual EnumInfo*  GetEnum (const char* name) = 0;
        virtual ClassInfo* GetClass(const char* name) = 0;

    public:
        virtual ~IRegister() {}

        virtual Meta::ClassResolver& GetClassResolver() = 0;
        virtual Memory::IMemoryBlock& GetMemory() = 0;

        virtual void LoadLayer  (ILayer* layer) = 0;
        virtual void UnloadLayer(ILayer* layer) = 0;

        // Used by generated code
        virtual void ResolveClass(Meta::detail::TResolver&& resolver) = 0;
    };

    namespace detail
    {
        extern IRegister* _registerInstance;
    }
    
    void SetupRTTI(Memory::IMemoryBlock& memory = Memory::GetDefaultBlock<Register>());
    void ShutdownRTTI();
    IRegister* GetRegister();
}

#endif // !YAGE_RTTI_REGISTER_H
