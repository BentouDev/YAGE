//
// Created by mrjaqbq on 06.03.16.
//

#ifndef VOLKHVY_CONTAINER_H
#define VOLKHVY_CONTAINER_H

#include <vector>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#include "Index.h"
#include "SafeDelete.h"
#include "MemoryBlock.h"

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
		//std::vector<object_t> 		 elements;
		//std::vector<Index<handle_t>> indices;
		Memory::MemoryBlockBase& _memory;
	//	Utils::List<Index<handle_t>> indices;
		std::vector<Index<handle_t>> indices;
		object_t* elements;

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
		inline explicit Container(Memory::MemoryBlockBase& memory, uint16_t size = 16) : _memory(memory), maxSize(size)
		{
			//elements.reserve(maxSize);
			elements = (object_t*)memory.allocate(sizeof(object_t) * maxSize, alignof(object_t), DEBUG_SOURCE_INFO);
			//elements = (object_t*)malloc(sizeof(object_t) * maxSize);
			memset(elements, 0, sizeof(object_t) * maxSize);
			indices.reserve(maxSize);

			initialize(this);
		}

		inline virtual ~Container()
		{
			clear();
			//elements.clear();
			//Memory::SafeFreeArray(elements, maxSize);
			for(uint32_t i = 0; i < maxSize; i++)
			{
				unsigned char* current, result = 0;
				object_t* start = &elements[i];
				object_t* end 	= start + 1;

				for(current  = reinterpret_cast<unsigned char*>(start);
					current != reinterpret_cast<unsigned char*>(end);
					current++)
				{
					result |= *current;
				}

				bool isZeroed = !result;
				if(!isZeroed)
				{
					elements[i].~object_t();
					memset(&elements[i], 0, sizeof(object_t));
				}
			}

			_memory.deallocate(elements);
			indices.clear();
		}

		template <typename... Args>
		inline auto create(Args... args) -> handle_t
		{
			Index<handle_t> &in = indices[freelistStart];
			freelistStart = in.next;

			Trait::incrementLiveId(in);

			in.index = elementCount++;
			in.valid |= 1;

			//typename std::vector<object_t>::iterator itr = elements.emplace(elements.begin() + in.index, args...);
			new (&elements[in.index]) object_t(args...);
			object_t& o = elements[in.index];
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

		/*inline auto activate(handle_t handle) noexcept -> void
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
		}*/

		inline auto contains(handle_t handle) const noexcept -> bool
		{
			const Index<handle_t> &in = indices[Trait::getIndex(handle)];
			return in.handle.key == handle.key && (in.valid & 1) && in.index != maxSize;
		}

		inline auto get(handle_t handle) const -> object_t&
		{
			const Index<handle_t> &in = indices[Trait::getIndex(handle)];
			return elements[in.index];
		}

		inline auto operator[](handle_t handle) -> object_t&
		{
			return get(handle);
		}

		inline auto operator[](handle_t handle) const -> const object_t&
		{
			return get(handle);
		}

		inline auto operator[](uint32_t index) -> object_t&
		{
			return elements[index];
		}

		inline auto operator[](uint32_t index) const -> const object_t&
		{
			return elements[index];
		}

		inline auto size() -> uint16_t
		{
			return elementCount;
		}

		inline auto clear() -> void
		{
			// todo: check what vector::clear exactly does (in regards to reserved memory) and do accordingly
			for(int i = 0; i < elementCount; i++)
			{
				object_t& o = elements[i];
				remove(Trait::getHandle(o));
			}
		}
	};
}

#endif //VOLKHVY_CONTAINER_H
