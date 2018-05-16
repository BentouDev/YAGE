//
// Created by bentoo on 29.11.16.
//

#ifndef GAME_ASSERT_H
#define GAME_ASSERT_H

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <utility>

#include "DebugSourceInfo.h"

#ifdef _MSC_VER
#define YAGE_BREAK __debugbreak()
#elif __MINGW32__
#define YAGE_BREAK __builtin_trap()
#else
#include <csignal>
#define YAGE_BREAK raise(SIGTRAP)
#endif

#ifndef NDEBUG
#define YAGE_ASSERT(predicate, message, ... ) (predicate) ? ((void)0) \
 : (Utils::Assert(DEBUG_SOURCE_INFO, message, ##__VA_ARGS__), YAGE_BREAK, ((void)0))
#else
#define YAGE_ASSERT(predicate, message, ... ) (void(0))
#endif

namespace Utils
{
	struct Assert
	{
		static char buffer[256];

		template <typename ... Args>
		Assert(Utils::DebugSourceInfo info, const char* message, Args&& ... args)
		{
			std::snprintf(buffer, sizeof(buffer), message, std::forward<Args>(args)...);
			std::fprintf(stderr, "Assert failed!\n\tfile %s, line %zu,\nreason : %s",
						 info.file, info.line, buffer);
		}
	};
}

#endif //GAME_ASSERT_H
