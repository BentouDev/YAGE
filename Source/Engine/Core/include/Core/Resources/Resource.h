//
// Created by mrjaqbq on 06.03.16.
//

#ifndef YAGE_RESOURCE_H
#define YAGE_RESOURCE_H

#include <string>
#include <BaseObject.h>
#include <Utils/Handle.h>
#include <Utils/Attribute.h>
#include <Utils/CompileString.h>

namespace Core
{
    class IResource : public yage::SafeObject
    {
    public:
        using handle_t = Utils::RawHandle;

    protected:
        explicit IResource() : Name("none") { }

		IResource(IResource&& other) noexcept
			: yage::SafeObject(std::move(other))
            , Name(std::move(other.Name))
        { }

    public:
        explicit IResource(const IResource&) = delete;
		IResource& operator=(const IResource&) = delete;
		IResource& operator=(IResource&&) = delete;

        virtual ~IResource() = default;

		eastl::string Name;
    };
}

#endif //YAGE_RESOURCE_H
