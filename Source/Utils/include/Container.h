//
// Created by mrjaqbq on 06.03.16.
//

#ifndef VOLKHVY_CONTAINER_H
#define VOLKHVY_CONTAINER_H

#include "Index.h"
#include "SafeDelete.h"

namespace Utils
{
	template<typename Trait>
	class Container
	{
	public:
		using object_t = typename Trait::type;
		using handle_t = typename Trait::handle;
		using activeCondition = bool(*)(object_t&);

	private:
		std::vector<object_t> 		 elements;
		std::vector<Index<handle_t>> indices;

		uint16_t	freelistEnd;
		uint16_t	freelistStart;

		uint16_t	unactivelistEnd;
		uint16_t	unactivelistStart;

		uint16_t 	elementCount;
		uint16_t 	activeCount;

		const uint16_t maxSize;

		activeCondition _condition;

		static auto initialize(Container* container) -> void
		{
			container->elementCount = 0;
			container->activeCount = 0;

			for (unsigned i = 0; i < container->maxSize; ++i)
			{
				container->indices.emplace(container->indices.begin() + i);
				Trait::setIndex(container->indices[i], i);
				container->indices[i].next = i + 1;
			}

			container->unactivelistStart = 0;
			container->unactivelistEnd = 0;
			container->freelistStart = 0;
			container->freelistEnd = container->maxSize - 1;
		}

	public:
		virtual ~Container()
		{
			elements.clear();
			indices.clear();
		}

		Container(uint16_t size) : maxSize(size)
		{
			elements.reserve(maxSize);
			indices.reserve(maxSize);

			initialize(this);
		}

		template <typename... Args>
		auto create(Args... args) -> handle_t
		{
			Index<handle_t> &in = indices[freelistStart];
			freelistStart = in.next;

			Trait::incrementLiveId(in);

			in.index = elementCount++;
			in.valid |= 1;

			auto itr = elements.emplace(elements.begin() + in.index, args...);
			object_t& o = (*itr);
			Trait::setHandle(o, in.handle);

			return Trait::getHandle(o);
		}

		inline void remove(handle_t handle)
		{
			Index<handle_t> &in = indices[Trait::getIndex(handle)];
			object_t &o = elements[in.index];

			Trait::cleanUp(o);
			Trait::swap(o, elements[--elementCount]);

			indices[Trait::getIndex(Trait::getHandle(o))].index = in.index;
			in.index = elementCount;
			in.valid &= 0;

			auto oldIndex = Trait::getIndex(handle);
			indices[freelistEnd].next = oldIndex;
			freelistEnd = oldIndex;
		}

		inline auto activate(handle_t handle) noexcept -> void
		{
			Index<handle_t> &in = indices[Trait::getIndex(handle)];
			object_t &o = elements[in.index];
			in.active &= 1;
		}

		inline auto deactivate(handle_t handle) noexcept -> void
		{
			Index<handle_t> &in = indices[Trait::getIndex(handle)];
			object_t &o = elements[in.index];
			in.active &= 0;
		}

		inline auto contains(handle_t handle) const noexcept -> bool
		{
			const Index<handle_t> &in = indices[Trait::getIndex(handle)];
			return in.handle.key == handle.key && (in.valid & 1) && in.index != maxSize;
		}

		inline auto get(handle_t handle) -> object_t&
		{
			const Index<handle_t> &in = indices[Trait::getIndex(handle)];
			return elements[in.index];
		}

		inline auto operator[](handle_t handle) -> object_t&
		{
			return get(handle);
		}

		inline auto operator[](uint32_t index) const -> object_t&
		{
			return elements[index];
		}
	};
}

#endif //VOLKHVY_CONTAINER_H
