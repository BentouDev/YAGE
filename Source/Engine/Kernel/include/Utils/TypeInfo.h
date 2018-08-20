//
// Created by mrjaqbq on 13.05.16.
//

#ifndef GAME_TYPEINFO_H
#define GAME_TYPEINFO_H

#include <cstdint>
#include <atomic>
#include <string>
#include "Assert.h"

#define AS_STRING(arg) #arg

typedef uint8_t type_t;

namespace
{
	class TypeCounter
	{
	public:
		static std::atomic<type_t> _lastTypeId;
	};
}

std::atomic<type_t> (::TypeCounter::_lastTypeId) { 0 };

template <typename T>
class TypeInfo
{
	explicit TypeInfo() = delete;
	explicit TypeInfo(const TypeInfo&) = delete;
	explicit TypeInfo(TypeInfo&&) = delete;

public:
	static auto id() noexcept -> type_t
	{
		static type_t id = ::TypeCounter::_lastTypeId++;
		YAGE_ASSERT(id != 255, "TypeInfo : Danger, typeId exceded uint8_t maximum!");
		return id;
	};

	static auto name() -> std::string
	{
		return AS_STRING(T);
	}

	constexpr static auto cName() -> char const*
	{
		return AS_STRING(T);
	}
};

#endif //GAME_TYPEINFO_H
