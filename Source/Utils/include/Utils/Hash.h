//
// Created by bentoo on 22.01.17.
//

#ifndef YAGE_HASH_H
#define YAGE_HASH_H

#include <cstdint>

namespace Utils
{
	std::uint32_t Hash(const char* string);
	std::uint32_t Hash(const char* string, std::uint32_t seed);
}

#endif //YAGE_HASH_H
