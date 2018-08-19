#ifndef YAGE_SYSTEM_DYNAMIC_LIBARY_H
#define YAGE_SYSTEM_DYNAMIC_LIBARY_H

#include <Utils/String.h>

namespace Core
{
    class IDynamicLibrary // Reference counting
    {
        // Disable copy constructor
        IDynamicLibrary(const IDynamicLibrary&) = delete;

    protected:
        explicit IDynamicLibrary(const Utils::String& name);

    public:
        virtual ~IDynamicLibrary();

        // Enable move semantics
        IDynamicLibrary(IDynamicLibrary&& );

        template <typename TProc>
        TProc* GetProcedure(const char* name)
        {
            return reinterpret_cast<TProc*>(GetProcedure(name));
        }

    protected:
        virtual void* GetProcedure(const char* name) = 0;
    };
}

#endif // !YAGE_SYSTEM_DYNAMIC_LIBARY_H