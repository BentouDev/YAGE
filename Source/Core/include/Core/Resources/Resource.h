//
// Created by mrjaqbq on 06.03.16.
//

#ifndef YAGE_RESOURCE_H
#define YAGE_RESOURCE_H

#include <string>
#include <Utils/Index.h>
#include <Utils/TypeInfo.h>
#include <Utils/Handle.h>

#define DECL_RESOURCE(name) class name : public Core::Resource<name>

namespace Core
{
	template<typename Res>
	class Resource
	{
	public:
		using handle_t = Utils::Handle<Res>;

	protected:
		explicit Resource() : Handle(), Name("none") { }
		Resource(Resource&&) : Handle(), Name("none") { }

	public:
		explicit Resource(const Resource&) = delete;
		Resource& operator=(const Resource&) = delete;
		Resource& operator=(Resource&&) = delete;

		virtual ~Resource(){ }

	//	virtual auto swap(Res& other) noexcept -> void = 0;
	//	virtual auto cleanUp() noexcept -> void = 0;

		handle_t 	Handle;

		// Todo : package reference
		std::string Name;
	};
}

#endif //YAGE_RESOURCE_H
