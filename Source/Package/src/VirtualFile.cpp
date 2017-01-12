//
// Created by Bentoo on 2017-01-12.
//

#include "Package/VirtualFile.h"
#include <Utils/SafeDelete.h>
#include <Utils/Assert.h>

namespace Yage
{
	namespace Package
	{
		VirtualFile::VirtualFile(void* data, std::uint32_t size)
			: _data(data), _size(size)
		{

		}

		VirtualFile::~VirtualFile()
		{
			YAGE_ASSERT(_data != nullptr, "VirtualFile : Leaked file with size in bytes '%'.");
		}
	}
}