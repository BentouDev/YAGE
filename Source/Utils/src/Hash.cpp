//
// Created by bentoo on 22.01.17.
//

#include "Utils/Hash.h"
#include <MurmurHash3.h>
#include <cstring>

namespace Utils
{
	std::uint32_t Hash(const char* string)
	{
		return Hash(string, 0);
	}

	std::uint32_t Hash(const char* string, std::uint32_t seed)
	{
		std::uint32_t result = 0;
		MurmurHash3_x86_32(string, (int) std::strlen(string), seed, &result);
		return result;
	}
}
