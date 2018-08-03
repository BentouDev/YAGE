//
// Created by mrjaqbq on 01.06.16.
//

#ifndef GAME_FILE_H
#define GAME_FILE_H

#include <string>
#include <fstream>

namespace Utils
{
	class File
	{
	public:
		std::string Path;
		std::string Extension;
		std::fstream Stream;
		std::streampos Position;
	};
}

#endif //GAME_FILE_H
