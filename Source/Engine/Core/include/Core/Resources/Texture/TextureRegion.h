//
// Created by bentoo on 11.05.17.
//

#ifndef YAGE_TEXTUREREGION_H
#define YAGE_TEXTUREREGION_H

#include <Platform/Graphics/Rectangle.h>

namespace Resources
{
	class Texture;

	struct TextureRegion
	{
		TextureRegion(Gfx::rect_t coords,
					  std::uint8_t layer = 0)
		: coordinates(coords), layer(layer)
		{

		}

		Gfx::rect_t 	coordinates;
		std::uint8_t	layer;
	};
}

#endif //YAGE_TEXTUREREGION_H
