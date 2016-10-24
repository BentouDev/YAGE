//
// Created by mrjaqbq on 07.03.16.
//

#ifndef YAGE_COLOR_H
#define YAGE_COLOR_H

#include <stdint.h>

namespace Utils
{
	union Color
	{
		uint32_t Key;

		struct
		{
			uint8_t R;
			uint8_t G;
			uint8_t B;
			uint8_t A;
		};
	};
}

#endif //YAGE_COLOR_H
