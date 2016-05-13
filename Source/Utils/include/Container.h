//
// Created by mrjaqbq on 06.03.16.
//

#ifndef VOLKHVY_CONTAINER_H
#define VOLKHVY_CONTAINER_H

#include "Index.h"

namespace Utils
{
	template<typename Trait>
	class Container
	{
		using object_t = typename Trait::type;
		using handle_t = typename Trait::handle;

		object_t* 			elements;
		Index<handle_t>*	indices;

		uint16_t	_freelist_enqueue;
		uint16_t	_freelist_dequeue;

		uint16_t 	elementCount;

		const uint16_t maxSize;

		static auto initialize(Container* container) -> void
		{
			container->elementCount = 0;
			for (unsigned i = 0; i < container->maxSize; ++i) {
				Trait::setIndex(container->indices[i], i);
				container->indices[i].next = i + 1;
			}

			container->_freelist_dequeue = 0;
			container->_freelist_enqueue = container->maxSize - 1;
		}

	public:
		Container(uint16_t size) : maxSize(size)
		{
			elements = new object_t[size];
			initialize(this);
		}

		template <uint16_t size>
		Container(char (*memory)[size]) : maxSize(size)
		{
			elements = new (memory) object_t[size];
			initialize(this);
		}

		template <typename... Args>
		auto create(Args... args) -> handle_t
		{
			Index<handle_t> &in = indices[_freelist_dequeue];
			_freelist_dequeue = in.next;

			Trait::incrementLiveId(in);

			in.index = elementCount++;

			object_t &o = new (elements[in.index]) object_t(args...);
			Trait::setHandle(o, in.handle);

			return Trait::getHandle(o);
		}

		inline void remove(handle_t handle)
		{
			Index<handle_t> &in = indices[Trait::getIndex(handle)];

			object_t &o = elements[in.index];
			Trait::swap(o, elements[--elementCount]);
			indices[Trait::getIndex(handle)].index = in.index;

			in.index = maxSize;
			indices[_freelist_enqueue].next = Trait::getIndex(handle);
			_freelist_enqueue = Trait::getIndex(handle);
		}

		inline auto contains(handle_t handle) const noexcept -> bool
		{
			Index<handle_t> &in = indices[Trait::getIndex(handle)];
			return in.handle == handle && in.index != maxSize;
		}

		inline auto get(handle_t handle) const -> object_t&
		{
			return elements[indices[Trait::getIndex(handle)].index];
		}

		inline auto operator[](handle_t handle) const -> object_t&
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
