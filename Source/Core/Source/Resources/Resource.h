//
// Created by mrjaqbq on 06.03.16.
//

#ifndef VOLKHVY_RESOURCE_H
#define VOLKHVY_RESOURCE_H

#include <string>
#include "ResourceHandle.h"

#define RESOURCE(name) class name : Resource<name>

namespace Core
{
	template<typename T>
	class Resource
	{
	public:
		DECL_HANDLE(T);

		Resource() = delete;
		Resource(const Resource&) = delete;
		Resource(Resource&&) = delete;

		std::string Filename;
		std::string Name;
	};
}

#endif //VOLKHVY_RESOURCE_H
