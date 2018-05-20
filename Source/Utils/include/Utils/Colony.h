//
// Created by bentoo on 18.05.18.
//

#ifndef YAGE_COLONY_H
#define YAGE_COLONY_H

#include <cstdint>
#include "List.h"
#include "Index.h"

namespace Utils
{
    class IColony
    {
    protected:
        IColony() = default;

    public:
        virtual ~IColony() = default;
    };

    struct IndexInfo
    {
        std::uint32_t pos;
        std::uint32_t next;
        std::uint16_t liveId;
    };

    template <typename Trait>
    class Colony : public IColony
    {
        static constexpr uint32_t bad_pos = std::numeric_limits<uint32_t>::max();

        using object_t = typename Trait::object_t;
        using handle_t = typename Trait::handle_t;

        static_assert(std::is_move_constructible<object_t>::value,
                      "Colony : Trait type must be move constructible! (due to internal swap)");

        Memory::IMemoryBlock&  _memory;
        Utils::List<object_t>  _elements;
        Utils::List<IndexInfo> _lookup;

        uint32_t freeListStart;

        bool isFreeListEmpty() const
        { return freeListStart != bad_pos; }

    public:
        explicit Colony(Memory::IMemoryBlock& memory, std::size_t reserved_size = 0)
           : _memory(memory), _elements(_memory, reserved_size), _lookup(_memory, reserved_size)
        {
            freeListStart = bad_pos;
        }

        ~Colony()
        {
            clear();
        }

        void clear()
        {
            _elements.clear();

            for (auto i = 0; i < _lookup.size(); i++)
            {
                auto& index = _lookup[i];
                index.liveId++;
                index.pos  = i;
                index.next = i + 1;
            }

            if (_lookup.empty())
                return;

            freeListStart = 0;
            _lookup.back().next = bad_pos;
        }

        handle_t insert(object_t&& original)
        {
            if (!isFreeListEmpty())
            {
                auto info_idx = freeListStart;
                auto& index   = _lookup[freeListStart];
                freeListStart = index.next;
                index.liveId++;

                auto& obj = _elements.emplace(std::move(original));

                Trait::setHandle(obj, index.liveId, info_idx);

                return Trait::getHandle(obj);
            }
            else
            {
                _lookup.emplace();
                auto& index  = _lookup.back();
                index.pos    = uint32_t(_elements.size());
                index.liveId = 1;
                index.next   = bad_pos;

                auto& obj = _elements.emplace(std::move(original));

                Trait::setHandle(obj, index.liveId, _lookup.size() - 1);

                return Trait::getHandle(obj);
            }
        }

        template <typename ... Args>
        handle_t emplace(Args&&... args)
        {
            if (freeListStart < _lookup.size())
            {
                auto info_idx = freeListStart;
                auto& index   = _lookup[freeListStart];
                freeListStart = index.next;
                index.liveId++;

                YAGE_ASSERT(index.pos == _elements.size(), "Fun!");

                auto& obj = _elements.emplace(std::forward<Args>(args)...);

                Trait::setHandle(obj, index.liveId, info_idx);

                return Trait::getHandle(obj);
            }
            else
            {
                _lookup.emplace();
                auto& index  = _lookup.back();
                index.pos    = uint32_t(_elements.size());
                index.liveId = 1;
                index.next   = bad_pos;

                auto& obj = _elements.emplace(std::forward<Args>(args)...);

                Trait::setHandle(obj, index.liveId, _lookup.size() - 1);

                return Trait::getHandle(obj);
            }
        }

        object_t* tryGet(handle_t handle)
        {
            return contains(handle) ? &get(handle) : nullptr;
        }

        const object_t* tryGet(handle_t handle) const
        {
            return contains(handle) ? &get(handle) : nullptr;
        }

        template <typename ... Args>
        std::tuple<handle_t, object_t&> emplaceAndGet(Args&&... args)
        {
            handle_t handle = emplace(std::forward<Args>(args)...);
            return { handle, get(handle) };
        }

        object_t& get(handle_t handle)
        {
            YAGE_ASSERT(contains(handle), "Colony : Cannot get with invalid handle!");
            auto& index = _lookup[Trait::getIndex(handle)];
            return _elements[index.pos];
        }

        const object_t& get(handle_t handle) const
        {
            YAGE_ASSERT(contains(handle), "Colony : Cannot get with invalid handle!");
            auto& index = _lookup[Trait::getIndex(handle)];
            return _elements[index.pos];
        }

        bool contains(handle_t handle) const
        {
            auto idx_pos = Trait::getIndex(handle);
            if (idx_pos < 0 || idx_pos >= _lookup.size())
                return false;

            const IndexInfo& in = _lookup[idx_pos];
            return in.liveId == handle.liveId && in.pos < _elements.size();
        }

        std::size_t erase(handle_t handle)
        {
            if (!contains(handle)) return 0;

            auto  pos   = Trait::getIndex(handle);
            auto& index = _lookup[pos];
            auto& obj   = _elements[index.pos];

            auto& last_obj   = _elements.back();
            auto  last_pos   = Trait::getIndex(Trait::getHandle(last_obj));
            auto& last_index = _lookup[last_pos];

            obj.~object_t();

            if (pos != last_pos)
            {
                new (&obj) object_t(std::move(last_obj));

                Trait::setHandle(obj, last_index.liveId, last_pos);
                last_index.pos = index.pos;
            }

            _elements.resize(_elements.size() - 1);

            index.pos     = _elements.size();
            index.next    = freeListStart;
            freeListStart = Trait::getIndex(handle);

            return 1;
        }

        object_t& operator[](handle_t handle)
        { return get(handle); }

        const object_t& operator[](handle_t handle) const
        { return get(handle); }

        object_t& at(std::size_t index)
        { return _elements[index]; }

        const object_t& at(std::size_t index) const
        { return _elements[index]; }

        object_t& operator[](std::size_t index)
        { return at(index); }

        const object_t& operator[](std::size_t index) const
        { return at(index); }

        object_t& front()
        { return _elements.front(); }

        object_t& front() const
        { return _elements.front(); }

        object_t& back()
        { return _elements.back(); }

        object_t& back() const
        { return _elements.back(); }

        object_t* begin()
        { return _elements.begin(); }

        object_t* begin() const
        { return _elements.begin(); }

        object_t* end()
        { return _elements.end(); }

        object_t* end() const
        { return _elements.end(); }

        size_t size() const noexcept
        { return _elements.size(); }

        size_t capacity() const noexcept
        { return _elements.capacity(); }
    };
}

#endif //YAGE_COLONY_H
