//
// Created by mrjaqbq on 15.05.16.
//

#ifndef GAME_DEFAULTTRAIT_H
#define GAME_DEFAULTTRAIT_H

#include <cstdint>
#include <Index.h>
#include <Handle.h>

namespace Utils
{
	template <typename T>
	class DefaultTrait
	{
	public:
		using type = T;
		using handle = Utils::Handle<T>;

		inline static void cleanUp(type& first) { first.cleanUp(); }

		inline static void swap(type& first, type& second) noexcept { first.swap(second); }

		inline static void incrementLiveId(Utils::Index<handle>& index) noexcept { index.handle.liveId++; }

		inline static void setIndex(Utils::Index<handle>& index, uint16_t i) noexcept { index.handle.index = i; }

		inline static uint16_t getIndex(Utils::Index<handle>& index) noexcept { return index.handle.index; }

		inline static uint16_t getIndex(handle _handle) noexcept { return _handle.index; }

		inline static void setHandle(type& obj, handle& _handle) noexcept { obj.Handle = _handle; }

		inline static handle getHandle(type& obj) noexcept { return obj.Handle; }
	};
}

#endif //GAME_DEFAULTTRAIT_H
