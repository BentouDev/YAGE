//
// Created by mrjaqbq on 07.03.16.
//

#ifndef YAGE_COLOR_H
#define YAGE_COLOR_H

#include <stdint.h>

namespace Utils
{
	struct Color
	{
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			: R(r), G(g), B(b), A(a)
		{ }

		Color(uint8_t r, uint8_t g, uint8_t b)
			: R(r), G(g), B(b), A(255)
		{ }

		Color()
			: R(255), G(255), B(255), A(255)
		{ }

		union
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
	};
}

#endif //YAGE_COLOR_H
