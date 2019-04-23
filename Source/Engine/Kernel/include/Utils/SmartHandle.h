//
// Created by bentoo on 19.05.18.
//

#ifndef YAGE_SMARTHANDLE_H
#define YAGE_SMARTHANDLE_H

#include "Colony.h"

namespace Utils
{
    template <typename Type>
    class SmartHandle
    {
        using object_t = typename Type;
        using handle_t = typename Utils::Handle<Type>;
		using colony_t = typename Colony<Type>;

		colony_t* Owner;
        handle_t  Handle;

    public:
        SmartHandle() 
            : Owner(nullptr)
            , Handle()
        { }

        SmartHandle(colony_t* colony, handle_t handle)
            : Owner(colony)
            , Handle(handle)
        { }

        object_t* get()
        { return Owner ? Owner->tryGet(Handle) : nullptr; }

        const object_t* get() const
        { return Owner ? Owner->tryGet(Handle) : nullptr; }

        object_t& operator*()
        { return Owner->get(Handle); }

        const object_t& operator*() const
        { return Owner->get(Handle); }

        object_t* operator->()
        { return Owner->tryGet(Handle); }

        const object_t* operator->() const
        { return Owner->tryGet(Handle); }

        explicit operator object_t&()
        { return Owner->get(Handle); }

        explicit operator object_t&() const
        { return Owner->get(Handle); }

        explicit operator bool() const
        { return Owner ? Owner->contains(Handle) : false; }
    };

    template <typename Type, typename ... Args>
	static SmartHandle<Type> make_handle(Colony <Type> * colony, Args&& ... args)
    {
        auto handle = colony->emplace(std::forward<Args>(args)...);
        return SmartHandle<Type>(colony, handle);
    }
}

#endif //YAGE_SMARTHANDLE_H
