//
// Created by Bentoo on 2017-04-03.
//

#include <new>
#include <Core/Gfx/Sprite.h>

namespace Gfx
{
	Sprite::Sprite()
		: rect(0, 0, 1, 1), texRect(0, 0, 1, 1), pivot(0.5f, 0.5f), scale(1), rotation(0), color(255,255,255,255)
	{ }

	Sprite::Sprite(Rectangle<float> rect)
		: rect(rect), texRect(0, 0, 1, 1), scale(1), rotation(0), color(255,255,255,255)
	{ }

	Sprite::Sprite(Rectangle<float> rect, Rectangle<float> texRect, glm::vec2 pivot, float scale, float rotation, Utils::Color color)
		: rect(rect), texRect(texRect), pivot(pivot), scale(scale), rotation(rotation), color(color)
	{ }

	Sprite::~Sprite()
	{ }

	void Sprite::fillVertexData(SpriteVertex (&array)[6], Rectangle<float> rect,
								Rectangle<float> texRect, Utils::Color color)
	{
		auto br = rect.getBottomRight();
		auto tr = rect.getTopRight();
		new (&array[0]) SpriteVertex(rect.getBottomRight(), texRect.getBottomRight(), color);
		new (&array[1]) SpriteVertex(rect.getTopRight(),    texRect.getTopRight(),    color);
		new (&array[2]) SpriteVertex(rect.getBottomLeft(),  texRect.getBottomLeft(),  color);

		new (&array[3]) SpriteVertex(rect.getBottomLeft(),  texRect.getBottomLeft(),  color);
		new (&array[4]) SpriteVertex(rect.getTopRight(),    texRect.getTopRight(),    color);
		new (&array[5]) SpriteVertex(rect.getTopLeft(),     texRect.getTopLeft(),     color);
	}
}
