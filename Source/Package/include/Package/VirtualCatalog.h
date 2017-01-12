//
// Created by Bentoo on 2017-01-12.
//

#ifndef YAGE_VIRTUALCATALOG_H
#define YAGE_VIRTUALCATALOG_H

#include <cstdint>

namespace Yage
{
	namespace Package
	{
		class VirtualCatalog
		{
		private:
			const char*		_name;
			std::uint32_t 	_chunkSize;
			std::uint32_t 	_childCatalougesCount;
			std::uint32_t 	_childFilesCount;

		public:

		};
	}
}

#endif //YAGE_VIRTUALCATALOG_H
