//
// Created by mrjaqbq on 06.03.16.
//

#ifndef YAGE_RESOURCE_H
#define YAGE_RESOURCE_H

#include <string>
#include <Utils/Index.h>
#include <Utils/Handle.h>
#include <Utils/Attribute.h>
#include <Utils/CompileString.h>

#define DECL_RESOURCE(name, ... ) YClass( __VA_ARGS__ );\
     class name : public Core::Resource<name>

namespace Core
{
    template<typename Res>
    class Resource
    {
    public:
        using handle_t = Utils::Handle<Res>;

    protected:
        explicit Resource() : Handle(), Name("none") { }

        Resource(Resource&& other) noexcept
            : Handle(std::move(other.Handle))
            , Name(std::move(other.Name))
        { }

    public:
        explicit Resource(const Resource&) = delete;
        Resource& operator=(const Resource&) = delete;
        Resource& operator=(Resource&&) = delete;

        virtual ~Resource() = default;

        std::string Name;

        handle_t Handle;
    };
}

#endif //YAGE_RESOURCE_H
