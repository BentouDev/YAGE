//
// Created by mrjaqbq on 06.03.16.
//

#ifndef YAGE_CONTAINER_H
#define YAGE_CONTAINER_H

#include <vector>
#include <cstdlib>
#include <malloc.h>
#include <memory.h>

#include "IContainer.h"
#include "Assert.h"
#include "List.h"
#include "Index.h"
#include "SafeDelete.h"
#include "MemoryBlock.h"

namespace Utils
{
	template<typename Trait>
	class Container : public IContainer
	{
	public:
		using object_t = typename Trait::object_t;
		using handle_t = typename Trait::handle_t;
		using activeCondition = bool(*)(object_t&);

		static_assert(std::is_move_constructible<object_t>::value,
					  "Trait type must be move constructible! (due to internal swap)");

	private:
		Memory::IMemoryBlock&			_memory;
		Utils::List<Index<handle_t>>	_indices;

		object_t* 	elements;

		uint16_t	freelistEnd;
		uint16_t	freelistStart;

		uint16_t	unactivelistEnd;
		uint16_t	unactivelistStart;

		uint16_t 	elementCount;
		uint16_t 	activeCount;

		const uint16_t maxSize;

		activeCondition _condition;

		void removeAllElements()
		{
			for (int i = 0; i < elementCount; i++)
			{
				object_t& o = elements[i];
				remove(Trait::getHandle(o));
			}
		}

		/*void destructElements()
		{
			for (uint32_t i = 0; i < maxSize; i++)
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
				if (!isZeroed)
				{
					elements[i].~object_t();
					memset(&elements[i], 0, sizeof(object_t));
				}
			}
		}*/

		static auto initialize(Container* container) -> void
		{
			container->elementCount = 0;
			container->activeCount = 0;

			for (unsigned i = 0; i < container->maxSize; ++i)
			{
				container->_indices.emplace();
				Trait::setIndex(container->_indices[i], i);
				container->_indices[i].next = i + 1;
			}

			container->unactivelistStart = 0;
			container->unactivelistEnd = 0;
			container->freelistStart = 0;
			container->freelistEnd = (uint16_t) (container->maxSize - 1);
		}

	public:
		inline explicit Container(Memory::IMemoryBlock& memory, uint16_t size = 16)
			: _memory(memory), _indices(_memory), maxSize(size)
		{
			_indices.reserve(maxSize);

			elements = (object_t*)memory.allocate(sizeof(object_t) * maxSize, alignof(object_t), DEBUG_SOURCE_INFO);
			memset(elements, 0, sizeof(object_t) * maxSize);

			initialize(this);
		}

		inline virtual ~Container()
		{
			removeAllElements();
			// destructElements();

			_memory.deallocate(elements);
			_indices.clear();
		}

		template <typename... Args>
		inline auto create(Args && ... args) -> handle_t
		{
			Index<handle_t> &in = _indices[freelistStart];
			freelistStart = in.next;

			Trait::incrementLiveId(in);

			in.index = elementCount++;
			in.valid |= 1;

			new (&elements[in.index]) object_t(args...);
			object_t& o = elements[in.index];
			Trait::setHandle(o, in.handle);

			return Trait::getHandle(o);
		}

		inline void remove(handle_t handle)
		{
			YAGE_ASSERT(contains(handle),
				   "Colony : Cannot remove element by invalid handle!");

			Index<handle_t> &in = _indices[Trait::getIndex(handle)];
			object_t &o = elements[in.index];

			--elementCount;
			object_t& lastElement = elements[elementCount];

			o.~object_t();
			new (&o) object_t(std::move(lastElement));

			_indices[Trait::getIndex(Trait::getHandle(o))].index = in.index;
			in.index = elementCount;
			in.valid &= 0;

			auto oldIndex = Trait::getIndex(handle);
			_indices[freelistEnd].next = oldIndex;
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
			const Index<handle_t> &in = _indices[Trait::getIndex(handle)];
			return in.handle.key == handle.key && (in.valid & 1) && in.index != maxSize;
		}

		inline auto get(handle_t handle) const -> object_t&
		{
#ifndef NDEBUG
			// ifndef added to help compiler inlining this in Release, ((void)0); would be ommited.
			YAGE_ASSERT(contains(handle),
						"Container : Cannot get element by invalid handle!");
#endif

			const Index<handle_t> &in = _indices[Trait::getIndex(handle)];
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

		inline auto at(uint32_t index) -> object_t&
		{
			return elements[index];
		}

		inline auto operator[](uint32_t index) -> object_t&
		{
			return at(index);
		}

		inline auto operator[](uint32_t index) const -> const object_t&
		{
			return at(index);
		}

		auto front() -> object_t&
		{
			return elements[0];
		}

		auto back() -> object_t&
		{
			return elements[elementCount - 1];
		}

		auto front() const -> object_t&
		{
			return elements[0];
		}

		auto back() const -> object_t&
		{
			return elements[elementCount - 1];
		}

		auto begin() -> object_t*
		{
			return elements;
		}

		auto end() -> object_t*
		{
			return elements + elementCount;
		}

		auto begin() const -> object_t*
		{
			return elements;
		}

		auto end() const -> object_t*
		{
			return elements + elementCount;
		}

		inline auto size() const noexcept -> uint16_t
		{
			return elementCount;
		}

		inline auto capacity() const noexcept -> std::uint16_t
		{
			return maxSize;
		}

		inline auto clear() -> void
		{
			removeAllElements();
		}
	};
}

#endif //YAGE_CONTAINER_H
