//
// Created by Bentoo on 2017-01-12.
//

#ifndef GAME_VIRTUALRECORD_H
#define GAME_VIRTUALRECORD_H

#include <Utils/List.h>
#include "Token.h"

namespace Yage
{
	namespace Package
	{
		class VirtualRecord
		{
		private:
			Token<4> 		_token;
			std::uint32_t 	_size;

		public:
			bool isFile();
			bool isCatalog();

			Utils::List<VirtualRecord> getChildNodes();

			std::uint32_t size()
			{
				return _size;
			}

			std::uint32_t size() const
			{
				return _size;
			}
		};
	}
}

#endif //GAME_VIRTUALRECORD_H
