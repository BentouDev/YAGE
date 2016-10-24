//
// Created by bentoo on 9/30/16.
//

#ifndef GAME_LIST_H
#define GAME_LIST_H

#include <new>
#include <cstring>
#include "DebugSourceInfo.h"
#include "MemoryBlock.h"

namespace Utils
{
	template <typename T>
	class List
	{
		Memory::IMemoryBlock &_memory;

		T* _elements;

		std::size_t _size;
		std::size_t _capacity;

		void destructElements()
		{
			for(unsigned i = 0; i < _size; i++)
			{
				_elements[i].~T();
			}
		}

		void cleanUp()
		{
			if(_elements != nullptr)
			{
				_memory.deallocate(_elements);
				_elements = nullptr;
			}
		}

		void realloc(std::size_t newSize)
		{
			if(_capacity == newSize)
				return;

			if (newSize < _size)
				resize(newSize);

			T* newPtr = nullptr;
			if(newSize > 0)
			{
				newPtr = (T*)_memory.allocate(sizeof(T) * newSize, alignof(T), DEBUG_SOURCE_INFO);

				if(_elements != nullptr)
				{
					memcpy(newPtr, _elements, sizeof(T) * _size);
				}
			}

			cleanUp();

			_elements = newPtr;
			_capacity = newSize;
		}

		void geometricGrow(std::size_t newSize)
		{
			// grow accoring to golden ration
			std::size_t bestSize = static_cast<std::size_t>(_capacity * 1.5f);

			if(bestSize < newSize)
			{
				bestSize = newSize;
			}

			realloc(bestSize);
		}

	public:
		inline explicit List(Memory::IMemoryBlock &memory)
			: _memory(memory), _size(0), _capacity(0), _elements(nullptr)
		{

		}

		inline List(Memory::IMemoryBlock &memory, std::size_t capacity)
			: _memory(memory), _size(0), _capacity(0), _elements(nullptr)
		{
			reserve(capacity);
		}

		inline List(const List& other)
			: _memory(other._memory), _size(0), _capacity(0), _elements(nullptr)
		{
			uint32_t otherSize = other._size;
			realloc(otherSize);
			memcpy(_elements, other._elements, sizeof(T) * otherSize);
			_size = otherSize;
		}

		inline List(List&& other)
			: _memory(other._memory), _size(other._size), _capacity(other._capacity), _elements(other._elements)
		{
			other._elements	= nullptr;
			other._size		= 0;
			other._capacity	= 0;
		}

		inline List<T>& operator=(const List<T>& other)
		{
			if(this != &other)
			{
				uint32_t otherSize = other._size;

				// we may have some data already
				destructElements();
				resize(otherSize);
				memcpy(_elements, other._elements, sizeof(T) * otherSize);
			}

			return *this;
		}

		inline List<T>& operator=(List<T>&& other) noexcept
		{
			if(this != &other)
			{
				// we may have some data already
				destructElements();
				cleanUp();

				_elements	= other._elements;
				_size 		= other._size;
				_capacity	= other._capacity;

				other._elements	= nullptr;
				other._size		= 0;
				other._capacity	= 0;
			}

			return *this;
		}

		inline virtual ~List() noexcept
		{
			destructElements();
			cleanUp();
		}

		void addMany(const List<T>& other)
		{
			std::size_t size = _size;
			resize(size + other.size());
			memcpy(_elements + size, other._elements, other.size() * sizeof(T));
		}

		template<typename ... Args>
		T& emplace(Args && ... args)
		{
			resize(_size + 1);

			T* result = &_elements[_size - 1];
			new (result) T(args ...);
			return	*result;
		};

		T& add(const T& other)
		{
			_size++;
			if((int)_capacity - (int)_size < 0)
			{
				resize(_size);
			}

			auto result = &_elements[_size - 1];
		//	new (result) T(other);
			(*result) = other;
			return *result;
		}

		T& add(T&& other)
		{
			_size++;
			if((int)_capacity - (int)_size < 0)
			{
				resize(_size);
			}

			auto result = &_elements[_size - 1];
		//	new (result) T(other);
			(*result) = std::move(other);
			return *result;
		}

		void erase(std::size_t index)
		{
			if(index >= _size)
				return;

			_elements[index].~T();

			std::size_t maxIndex = _size - 1;
			if(index < maxIndex)
			{
				memmove(&_elements[index], &_elements[index + 1], sizeof(T) * (maxIndex - index));
			}

			_size--;
		}

		void resize(std::size_t newSize)
		{
			if(newSize > _capacity)
				geometricGrow(newSize);
			_size = newSize;
		}

		void reserve(std::size_t newSize)
		{
			if(newSize > _capacity)
				realloc(newSize);
		}

		void clear()
		{
			for(unsigned i = 0; i < _size; i++)
			{
				_elements->~T();
			}

			resize(0);
		}

		T& front()
		{
			return _elements[0];
		}

		T& back()
		{
			return _elements[_size - 1];
		}

		T* begin()
		{
			return _elements;
		}

		T* end()
		{
			return _elements + _size;
		}

		std::size_t size() const
		{
			return _size;
		}

		std::size_t capacity() const
		{
			return _capacity;
		}

		inline T& operator[](std::size_t index)
		{
			return _elements[index];
		}

		inline const T& operator[](std::size_t index) const
		{
			return _elements[index];
		}
	};
}

#endif //GAME_LIST_H
