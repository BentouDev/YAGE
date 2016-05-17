//
// Created by mrjaqbq on 06.03.16.
//

#ifndef VOLKHVY_RESOURCE_H
#define VOLKHVY_RESOURCE_H

#include <string>
#include <Index.h>
#include <TypeInfo.h>
#include <Handle.h>

#define DECL_RESOURCE(name) class name : public Resource<name>

namespace Core
{
	template <typename Res>
	class ResourceManager;

	template<typename Res>
	class Resource
	{
		const void* _id;

	public:
		using handle_t = Utils::Handle<Res>;

	protected:
		explicit Resource(Resource&& other)
		{
			std::swap(Path, other.Path);
			std::swap(Name, other.Name);
		}

		virtual auto swap(Resource& other) noexcept -> void = 0;

	public:
		explicit Resource() = delete;
		explicit Resource(const Resource&) = delete;

		virtual ~Resource(){ }

		handle_t 	Handle;
		std::string Path;
		std::string Name;
	};
}

#endif //VOLKHVY_RESOURCE_H
