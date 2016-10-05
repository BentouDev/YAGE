//
// Created by bentoo on 9/27/16.
//

#ifndef GAME_DEBUGSOURCEINFO_H
#define GAME_DEBUGSOURCEINFO_H

#include <cstddef>

namespace Utils
{
	struct DebugSourceInfo
	{
		DebugSourceInfo(const char* _file, std::size_t _line)
				: file(_file), line(_line) { }

		const char* file;
		std::size_t line;
	};
}

#define DEBUG_SOURCE_INFO_EXPLICIT(file, line) Utils::DebugSourceInfo(file, line)
#define DEBUG_SOURCE_INFO DEBUG_SOURCE_INFO_EXPLICIT(__FILE__, __LINE__)

#endif //GAME_DEBUGSOURCEINFO_H
