//
// Created by mrjaqbq on 06.03.16.
//

#ifndef VOLKHVY_RESOURCE_H
#define VOLKHVY_RESOURCE_H

#include <string>
#include <Index.h>
#include <TypeInfo.h>
#include <Handle.h>

#define DECL_RESOURCE(name) class name : public Core::Resource<name>

namespace Core
{
	template<typename Res>
	class Resource
	{
		const void* _id;

	public:
		using handle_t = Utils::Handle<Res>;

	protected:
		explicit Resource() { }
		explicit Resource(Resource&& other)
		{
			std::swap(Path, other.Path);
			std::swap(Name, other.Name);
		}

	public:
		explicit Resource(const Resource&) = delete;

		virtual ~Resource(){ }

		virtual auto swap(Res& other) noexcept -> void = 0;
		virtual auto cleanUp() noexcept -> void = 0;

		handle_t 	Handle;
		std::string Path;
		std::string Name;
	};
}

#endif //VOLKHVY_RESOURCE_H
