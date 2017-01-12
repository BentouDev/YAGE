//
// Created by Bentoo on 2017-01-12.
//

#ifndef YAGE_PACKAGE_H
#define YAGE_PACKAGE_H

#include <cstdint>
#include "Token.h"

namespace Yage
{
	namespace Package
	{
		typedef Token<4> TypeToken;

		class Package
		{
			struct PackageHeader
			{

			};

		private:

		public:
			static Package* Load(const char* path) const;

			TypeToken FileToken    = TypeToken("FILE");
			TypeToken CatalogToken = TypeToken("CAT");
		};
	}
}

#endif //YAGE_PACKAGE_H