//
// Created by bentoo on 10/2/16.
//

#ifndef GAME_STRING_H
#define GAME_STRING_H

#include <type_traits>
#include <vector>
#include <cstring>
#include <cstdio>
#include <cctype>
#include "List.h"
#include "Slice.h"
#include "DefaultBlock.h"
#include <EASTL/string.h>

namespace Utils
{
	using String = eastl::string;
	class _broken_String : public _broken_List<char>
	{
	protected:
		inline void ensureNullTerminator()
		{
			if(back() != '\0')
			{
				add('\0');
				eraseAt(size() - 1);
			}
		}

		void onAddElement() override
		{
			ensureNullTerminator();
		}

	public:
		inline explicit _broken_String(Memory::IMemoryBlock &memory)
			: _broken_List(memory)
		{ }

		inline _broken_String(const _broken_String& other)
			: _broken_List(other)
		{ }

		inline _broken_String(_broken_String&& other)
			: _broken_List(other)
		{ }

		inline _broken_String(Memory::IMemoryBlock& memory, const char* string)
			: _broken_List(memory)
		{
			append(string);
		}

		inline _broken_String(Memory::IMemoryBlock& memory, Utils::Slice<char>& slice)
			: _broken_List(memory)
		{
			append(slice.begin(), slice.size());
		}

		inline _broken_String()
			: _broken_List(Memory::GetDefaultBlock<String>())
		{

		}

		inline _broken_String(const char* string)
			: _broken_List(Memory::GetDefaultBlock<String>())
		{
			append(string);
		}

		inline _broken_String(Utils::Slice<char>& slice)
			: _broken_List(Memory::GetDefaultBlock<String>())
		{
			append(slice.begin(), slice.size());
		}

		inline _broken_String& append(const _broken_String& str)
		{
			addMany(str);
			return *this;
		}

		inline _broken_String& append(const char* c_str, std::size_t count)
		{
			std::size_t oldSize = size();
			reserve(oldSize + count + 1);
			resize(oldSize + count);
			memcpy(&_elements[oldSize], c_str, count * sizeof(char));

			ensureNullTerminator();

			return *this;
		}

		inline _broken_String& append(const char* c_str)
		{
			std::size_t len = std::strlen(c_str);
			return append(c_str, len);
		}

		const char* c_str() const
		{
			return _broken_List::_elements;
		}

		template <typename T, std::size_t size = 32>
		_broken_String& stackSprintf(const char* format, const T& value)
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

		static void Tokenize(const String& string, Utils::List<Utils::Slice<char>>& list, const char* divider);
	};

	inline static _broken_String& operator<<(_broken_String& str, char c)
	{
		str.add(c);
		return str;
	}

	inline static _broken_String& operator<<(_broken_String& str, const char *s)
	{
		return str.append(s);
	}

	inline static _broken_String& operator<<(_broken_String& str, float f)
	{
		return str.stackSprintf("%g", f);
	}

	inline static _broken_String& operator<<(_broken_String& str, int32_t i)
	{
		return str.stackSprintf("%d", i);
	}

	inline static _broken_String& operator<<(_broken_String& str, uint32_t u)
	{
		return str.stackSprintf("%u", u);
	}

	inline static _broken_String& operator<<(_broken_String& str, uint64_t i)
	{
		// pad with zeroes, minimum 1 character, hex
		return str.stackSprintf("%01llx", i);
	}

	template <typename Str>
	static void SplitString(const char* str, std::vector<Str>& list, const char* divider)
	{
		char* obj	= strdup(str);
		char* temp	= strtok(obj, divider);
		while(temp)
		{
			char* token = strdup(temp);
			list.push_back(Str(token));
			temp = strtok(NULL, divider);
		}

		free(obj);
	}
}

#endif //GAME_STRING_H
