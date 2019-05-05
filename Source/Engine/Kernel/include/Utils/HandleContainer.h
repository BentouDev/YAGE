//
// Created by bentoo on 24.11.16.
//

#ifndef GAME_ARRAYCONTAINER_H
#define GAME_ARRAYCONTAINER_H

#include <tuple>

#include "TypeIndexList.h"
#include "List.h"
#include "Slice.h"
#include "Assert.h"

namespace Utils
{
	template<typename Handle>
	struct Index
	{
		Handle   handle;
		uint16_t index;
		uint16_t next;
		uint8_t  valid : 1;
	};

    template <typename TObject, typename T, uint8_t N>
    class HandleContainer
    {
    public:
        using object_t = typename TObject;
        using handle_t = typename Utils::Handle<TObject>;

		static_assert(std::is_base_of<yage::SafeObject, TObject>::value,
					  "T must derive from SafeObject!");

        static_assert(std::is_move_constructible<object_t>::value,
                      "Trait type must be move constructible! (due to internal swap)");

        static_assert(std::is_move_constructible<T>::value,
                      "T must be move constructible! (due to internal swap)");

    private:
        Memory::IMemoryBlock&			_memory;
        Utils::List<Index<handle_t>>	_indices;

        object_t*	_elements;
        T*			_array[N];

        uint16_t 	maxSize;

        uint16_t	freelistEnd;
        uint16_t	freelistStart;

        uint16_t 	elementCount;

        void removeAllElements()
        {
            for(int i = 0; i < elementCount; i++)
            {
                object_t& o = _elements[i];
                remove(Utils::handle_cast<TObject>(o.Handle));
            }
        }

        static auto initialize(HandleContainer* container) -> void
        {
            container->elementCount = 0;

            for (unsigned i = 0; i < container->maxSize; ++i)
            {
                container->_indices.emplace_back();
				container->_indices[i].index = i;
                container->_indices[i].next = i + 1;
            }

            container->freelistStart = 0;
            container->freelistEnd = (uint16_t) (container->maxSize - 1);
        }

    public:
        explicit HandleContainer(Memory::IMemoryBlock& memory, uint16_t size = 16)
            : _memory(memory), _indices(), _elements(nullptr), maxSize(size)// #NewAlloc
        {
            _indices.reserve(maxSize);

            _elements = reinterpret_cast<object_t*>(memory.allocate(sizeof(object_t) * maxSize, alignof(object_t),
                                                   DEBUG_SOURCE_INFO));

            for(int i = 0; i < N; i++)
            {
                _array[i] = reinterpret_cast<T*>(memory.allocate(sizeof(T) * maxSize, alignof(T),
                                                DEBUG_SOURCE_INFO));
            }

            memset(reinterpret_cast<void*>(_elements), 0, sizeof(object_t) * maxSize);

            initialize(this);
        }

        virtual ~HandleContainer()
        {
            removeAllElements();

            for(int i = 0; i < N; i++)
            {
                _memory.deallocate(_array[i]);
            }

            _memory.deallocate(_elements);
            _indices.clear();
        }

        template <typename... Args>
        inline auto create(Args && ... args) -> handle_t
        {
            Index<handle_t> &in = _indices[freelistStart];
            freelistStart = in.next;

			in.handle.key.liveId++;
            
            in.index = elementCount++;
            in.valid |= 1;

            new (&_elements[in.index]) object_t(args...);

            for(int i = 0; i < N; i++)
            {
                new (&_array[i][in.index]) T();
            }

            object_t& o = _elements[in.index];

			o.Handle.key.liveId = in.handle.key.liveId;
			o.Handle.key.index = in.handle.key.index;
            
            return Utils::handle_cast<TObject>(o.Handle);
        }

        inline void remove(handle_t handle)
        {
            YAGE_ASSERT(contains(handle),
                   "HandleContainer : Cannot remove element by invalid handle!");

            Index<handle_t> &in = _indices[handle.key.index];
            object_t &o = _elements[in.index];

            --elementCount;
            object_t& lastElement = _elements[elementCount];

            o.~object_t();
            new (&o) object_t(std::move(lastElement));

            for(int i = 0; i < N; i++)
            {
                T& old		= _array[i][in.index];
                T& lastT	= _array[i][elementCount];
                old.~T();
                new (&old) T(std::move(lastT));
            }

            _indices[o.Handle.key.index].index = in.index;
            in.index = elementCount;
            in.valid &= 0;

            auto oldIndex = handle.key.index;
            _indices[freelistEnd].next = oldIndex;
            freelistEnd = oldIndex;
        }

        inline auto contains(handle_t handle) const noexcept -> bool
        {
            const Index<handle_t> &in = _indices[handle.key.index];
            return in.handle.key.raw == handle.key.raw && (in.valid & 1) && in.index != maxSize;
        }

        inline auto get(handle_t handle) const -> object_t&
        {
#ifndef NDEBUG
            // ifndef added to help compiler inlining this in Release, ((void)0); would be ommited.
            YAGE_ASSERT(contains(handle),
                        "HandleContainer : Cannot get element by invalid handle!");
#endif

            const Index<handle_t> &in = _indices[handle.key.index];
            return _elements[in.index];
        }

        inline auto get(handle_t handle, uint8_t index) const -> T&
        {
#ifndef NDEBUG
            // ifndef added to help compiler inlining this in Release, ((void)0); would be ommited.
            YAGE_ASSERT(contains(handle),
                        "HandleContainer : Cannot get element by invalid handle!");
#endif

            const Index<handle_t> &in = _indices[handle.key.index];
            return _array[index][in.index];
        }

        inline auto set(handle_t handle, uint8_t index, T value)
        {
#ifndef NDEBUG
            // ifndef added to help compiler inlining this in Release, ((void)0); would be ommited.
            YAGE_ASSERT(contains(handle),
                        "HandleContainer : Cannot set element by invalid handle!");
#endif
            const Index<handle_t> &in = _indices[handle.key.index];
            T& old = _array[index][in.index];
            new (&old) T(std::move(value));
        }

        inline auto operator[](uint32_t index) -> Slice<T>&
        {
            Slice<T> result(_array[index], elementCount);
            return result;
        }

        inline auto operator[](uint32_t index) const -> const Slice<T>&
        {
            Slice<T> result(_array[index], elementCount);
            return result;
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

#endif //GAME_ARRAYCONTAINER_H
