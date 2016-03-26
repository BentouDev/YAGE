//
// Created by mrjaqbq on 06.03.16.
//

#ifndef VOLKHVY_CONTAINER_H
#define VOLKHVY_CONTAINER_H

#include "Handle.h"

#define INDEX_MASK 0xffff

namespace Utils
{
	/*template<typename T>
	class Container;

	template<typename T>
	struct Index
	{
		friend class Container<T>;

		Index() : index(0), next(0) { }

		Handle<T>	handle;
		uint16_t	index;
		uint16_t	next;

	private:
		void Set(uint16_t index)
		{
			handle.get().LiveId = index;
			next = index + 1;
		}
	};

	template<typename T>
	class Container
	{
	protected:
		void SetIndex(Index<T>& index, uint16_t value)
		{
			index.Set(value);
		}
	};

	template<typename T, uint16_t count = UINT16_MAX>
	class StaticContainer : public Container<T>
	{
	private:
		T Objects[count];

		uint16_t FreelistBegin;
		uint16_t FreelistEnd;
		uint16_t CurrentCount;

		Index<T> Indicies[count];

	public:
		StaticContainer() : CurrentCount(0)
		{
			for(uint16_t i = 0; i < count; ++i)
			{
				this->SetIndex(Indicies[i], i);
			}

			FreelistBegin = 0;
			FreelistEnd = count - 1;
		}

		inline bool Contains(const Handle<T> id) const
		{
			Index<T>& in = Indicies[id.rawHandle.LiveId & INDEX_MASK];
			return in.handle.rawHandle.LiveId == id.rawHandle.LiveId
				   && in.index != count;
		}

		inline T& Get(const Handle<T> id) const
		{
			auto i = id.rawHandle.LiveId & INDEX_MASK;
			return Objects[Indicies[i].index];
		}

		inline Handle<T> Create()
		{
			Index<T> &in = Indicies[FreelistBegin];

			in.handle.rawHandle.Index++;
			in.index = CurrentCount++;

			FreelistBegin = in.next;

			//T &o = Objects[in.index];
			// GetHandler<T>(o).liveID	= in.hand.liveID;
			// GetHandler<T>(o).id		= in.hand.id;
			return in.handle;//GetHandler<T>(o);
		}

		/*inline void Remove(Handle<T> id)
		{
			Index &in = Indicies[id.rawHandle.LiveId & INDEX_MASK];

			T &o = Objects[in.index];
			o = Objects[--CurrentCount];
			//GetHandler<T>(o)
			Indicies[in.handle.rawHandle.LiveId & INDEX_MASK].index = in.index;// o.id & INDEX_MASK].index = in.index;

			in.index = count;
			Indicies[FreelistEnd].next = id.rawHandle.LiveId & INDEX_MASK;
			FreelistEnd = id.rawHandle.LiveId & INDEX_MASK;
		}//

		inline auto operator[](const int i) const noexcept -> T&
		{
			return Objects[i];
		}

		inline auto operator[](const Handle<T> id) const noexcept -> T&
		{
			return Get(id);
		}
	};*/
}

#endif //VOLKHVY_CONTAINER_H
