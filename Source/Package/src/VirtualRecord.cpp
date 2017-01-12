//
// Created by Bentoo on 2017-01-12.
//

#include "Package/VirtualRecord.h"

namespace Yage
{
	namespace Package
	{
		bool VirtualRecord::isFile()
		{
			return _token == Package::FileToken;
		}
	}
}
