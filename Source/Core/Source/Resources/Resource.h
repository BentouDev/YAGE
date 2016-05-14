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

	template <typename Res>
	class ResourceTrait
	{
	public:
		using type = Res;
		using handle = Utils::Handle<Res>;

		inline static void cleanUp(type& first)
		{
			first.cleanUp();
		}

		inline static void swap(type& first, type& second) noexcept
		{
			first.swap(second);
		}

		inline static void incrementLiveId(Utils::Index<handle>& index) noexcept
		{
			index.handle.liveId++;
		}

		inline static void setIndex(Utils::Index<handle>& index, uint16_t i) noexcept
		{
			index.handle.index = i;
		}

		inline static uint16_t getIndex(Utils::Index<handle>& index) noexcept
		{
			return index.handle.index;
		}

		inline static void setHandle(type& obj, handle& _handle) noexcept
		{
			obj.Handle = _handle;
		}

		inline static handle getHandle(type& obj) noexcept
		{
			return obj.Handle;
		}
	};
}

#endif //VOLKHVY_RESOURCE_H
