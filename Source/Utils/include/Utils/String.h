//
// Created by bentoo on 10/2/16.
//

#ifndef GAME_STRING_H
#define GAME_STRING_H

#include <type_traits>
#include <cstring>
#include <cstdio>
#include <cctype>
#include "List.h"

namespace Utils
{
	class String : public List<char>
	{
	public:
		inline explicit String(Memory::IMemoryBlock &memory)
			: List(memory)
		{ }

		inline String(const String& other)
			: List(other)
		{ }

		inline String(String&& other)
			: List(other)
		{ }

		inline String(Memory::IMemoryBlock& memory, const char* string)
			: List(memory)
		{
			append(string);
		}

		inline String& append(const String& str)
		{
			addMany(str);
			return *this;
		}

		inline String& append(const char* c_str, std::size_t count)
		{
			std::size_t oldSize = size();
			resize(oldSize + count);
			memcpy(begin() + oldSize, c_str, count * sizeof(char));
			return *this;
		}

		inline String& append(const char* c_str)
		{
			return append(c_str, std::strlen(c_str));
		}

		const char* c_str()
		{
			if(back() != '\0')
			{
				add('\0');
				erase(size() - 1);
			}

			return begin();
		}

		template <typename T, std::size_t size = 32>
		String& stackSprintf(const char* format, const T& value)
		{
			char buffer[size];
			std::snprintf(buffer, size, format, value);
			return append(buffer);
		}

		static bool isNullOrEmpty(String* s)
		{
			if(s == nullptr)
				return true;
			if(s->size() == 0)
				return true;

			for(auto ptr = s->begin(); ptr != s->end(); ptr++)
			{
				if(!std::isspace(*ptr))
					return false;
			}

			return true;
		}

		static bool isNullOrEmpty(const String& s)
		{
			if(s.size() == 0)
				return true;

			for(unsigned i = 0; i < s.size(); i++)
			{
				if(!std::isspace(s[i]))
					return false;
			}

			return true;
		}
	};

	inline static String& operator<<(String& str, char c)
	{
		str.add(c);
		return str;
	}

	inline static String& operator<<(String& str, const char *s)
	{
		return str.append(s);
	}

	inline static String& operator<<(String& str, float f)
	{
		return str.stackSprintf("%g", f);
	}

	inline static String& operator<<(String& str, int32_t i)
	{
		return str.stackSprintf("%d", i);
	}

	inline static String& operator<<(String& str, uint32_t u)
	{
		return str.stackSprintf("%u", u);
	}

	inline static String& operator<<(String& str, uint64_t i)
	{
		// pad with zeroes, minimum 1 character, hex
		return str.stackSprintf("%01llx", i);
	}
}

#endif //GAME_STRING_H
