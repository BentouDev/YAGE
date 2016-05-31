//
// Created by mrjaqbq on 06.03.16.
//

#ifndef VOLKHVY_RESOURCEMANAGER_H
#define VOLKHVY_RESOURCEMANAGER_H

#include <string>
#include <vector>
#include <queue>
#include <map>

#include "Resource.h"
#include <Container.h>
#include <Logger.h>
#include <Context.h>
#include <Gfx/Api/BaseApi.h>

namespace Gfx
{
	class BaseApi;
}

namespace Core
{
	class Engine;
	class Context;
	class ResourceManager;

	// todo: class for bin file, with offset and handle itself
	// typedef void (*typeBinLoader)(FileOffsetHandle, Core::Context&, Gfx::BaseApi&);
	typedef void (*typePathLoader)(std::string, std::string ext, Core::ResourceManager&, Core::Context&, Gfx::BaseApi&);

	struct PathLoadCommand
	{
		PathLoadCommand(std::string p, std::string e, type_t t)
			: path(p), ext(e), type(t)
		{ }

		std::string path;
		std::string ext;
		type_t type;
	};

	// regarding resources:
	// two classes per type: one with file and parsing data
	// and one with actual in-engine-usable data
	// file data will be stored here
	// it will contain file path and offset (maybe some metadata)
	class ResourceManager
	{
	public:
		auto createResource()
		{

		}

		auto getResource()
		{

		}
	};

	class ResourceLoader
	{
		Core::Context& ctx;
		Utils::borrowed_ptr<Gfx::BaseApi> api;

		Utils::borrowed_ptr<ResourceManager> resManager;
		Utils::borrowed_ptr<Core::Logger> logger;

		std::map<std::string, type_t> extensionMap;
		std::map<type_t, typePathLoader> pathTypeLoaders;

		std::queue<PathLoadCommand> pathLoadQueue;

	public:
		ResourceLoader(Context context, Utils::borrowed_ptr<Gfx::BaseApi> baseApi)
			: ctx(context), api(baseApi)
		{ }

		// take item from queue and load it
		// if there is no more things to load, return true
		auto pathLoadFromQueue() -> bool
		{
			if(pathLoadQueue.empty()) return true;

			auto& res = pathLoadQueue.front();
			pathTypeLoaders[res.type](res.path, res.ext, (*resManager), ctx, (*api));
			pathLoadQueue.pop();

			return false;
		}

		template <typename T>
		auto registerTypeFile(std::vector<std::string>& file_extensions, typePathLoader loader) -> void
		{
			type_t type = TypeInfo<T>::id();
			for(auto ext : file_extensions)
			{
				//todo: check if is already included and issue a warning
				extensionMap[ext] = type;
			}

			pathTypeLoaders[type] = loader;
		}

		auto extractExtension(std::string path) -> std::string
		{
			std::string result;

			auto pos = path.find_last_of('.');
			if(pos != path.length())
			{
				result = path.substr(pos + 1);
			}
			//todo: issue a warning

			return result;
		}

		auto enqueueLoadFile(std::string path, std::string ext) -> void
		{
			auto itr = extensionMap.find(ext);
			if(itr != extensionMap.end())
			{
				pathLoadQueue.emplace(path, ext, extensionMap[ext]);
			}
			else
			{
				logger->Default->warn("Extension '{}' is not registred", ext);
			}
		}

		auto enqueueLoadFile(std::string path) -> void
		{
			std::string ext = extractExtension(path);
			if(!ext.empty())
			{
				enqueueLoadFile(path, ext);
			}
			else
			{
				logger->Default->warn("Unable to extract extension from filename '{}'", path);
			}
		}
	};

	/*template<class Trait>
	class ResourceManager
	{
		using handle_t = typename Trait::handle;
		Utils::Container<Trait> container;

		// load resource from file
		// load resource from binary data

		template <typename... Args>
		auto create(Args... args) -> handle_t
		{
			return container.create(args...);
		}

		auto getContainer() -> Utils::Container<Trait>&
		{
			return container;
		}
	};*/
}

#endif //VOLKHVY_RESOURCEMANAGER_H
