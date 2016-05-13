//
// Created by mrjaqbq on 06.03.16.
//

#ifndef VOLKHVY_RESOURCE_H
#define VOLKHVY_RESOURCE_H

#include <string>
#include <Index.h>
#include "Handle.h"

#define DECL_RESOURCE(name) class name : public Resource<name>

namespace Core
{
	template <typename Res>
	class ResourceManager;

	template<typename Res>
	class Resource
	{
		using handle_t = Utils::ResourceHandle<Res>;

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

	template <typename Res>
	class ResourceTrait
	{
	public:
		using type = Res;
		using handle = Utils::ResourceHandle<Res>;

		inline static auto swap(type& first, type& second) const noexcept -> void
		{
			first.swap(second);
		}

		inline static auto incrementLiveId(handle _handle) const noexcept -> void
		{
			handle.liveId++;
		}

		inline static auto setIndex(Utils::Index<handle> index, uint16_t i) const noexcept -> void
		{
			index.handle.index = i;
		}

		inline static auto getIndex(Utils::Index<handle> index) const noexcept -> uint16_t
		{
			return index.handle.index;
		}

		inline static auto setHandle(type& obj, handle _handle) const noexcept -> void
		{
			obj.Handle = _handle;
		}

		inline static auto getHandle(type& obj) const noexcept -> handle
		{
			return obj.Handle;
		}
	};
}

#endif //VOLKHVY_RESOURCE_H
