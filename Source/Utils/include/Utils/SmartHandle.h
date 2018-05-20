//
// Created by bentoo on 19.05.18.
//

#ifndef YAGE_SMARTHANDLE_H
#define YAGE_SMARTHANDLE_H

#include "Colony.h"

namespace Utils
{
    template <typename Trait>
    class SmartHandle
    {
        using object_t = typename Trait::object_t;
        using handle_t = typename Trait::handle_t;

        Colony<Trait>* Owner;
        handle_t       Handle;

    public:
        SmartHandle() = default;

        SmartHandle(Colony<Trait>* colony, handle_t handle)
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

    template <typename Trait, typename ... Args>
    static SmartHandle<Trait> make_handle(Colony<Trait>* colony, Args&&... args)
    {
        auto handle = colony->emplace(std::forward<Args>(args)...);
        return SmartHandle<Trait>(colony, handle);
    }
}

#endif //YAGE_SMARTHANDLE_H
