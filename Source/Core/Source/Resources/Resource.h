//
// Created by mrjaqbq on 06.03.16.
//

#ifndef VOLKHVY_RESOURCE_H
#define VOLKHVY_RESOURCE_H

#include <string>
#include "Handle.h"
#include "ResourceManager.h"

#define DECL_RESOURCE(name) class name : Resource<name>

namespace Core
{
	template<typename T>
	class Resource
	{
	public:
		using Handle = Utils::Handle<T>;
		using Manager = ResourceManager<T>;

		explicit Resource() = delete;
		explicit Resource(const Resource&) = delete;
		explicit Resource(Resource&&) = delete;

		virtual ~Resource(){ }

		std::string Filename;
		std::string Name;
	};
}

#endif //VOLKHVY_RESOURCE_H
