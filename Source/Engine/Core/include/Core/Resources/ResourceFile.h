//
// Created by mrjaqbq on 01.06.16.
//

#ifndef GAME_RESOURCEFILE_H
#define GAME_RESOURCEFILE_H

#include <Utils/File.h>
#include <Utils/Handle.h>

namespace Core
{
    YClass();
	class ResourceFile
	{
		Utils::RawHandle handle;
		Utils::File file;
	};

	class FileTrait
	{

	};
}

#endif //GAME_RESOURCEFILE_H
