//
// Created by Bentoo on 2017-04-03.
//

#include <new>
#include <Core/Gfx/Sprite.h>

namespace Gfx
{
	Sprite::Sprite()
		: rect(0, 0, 1, 1), textureRegion(rect_t(0, 0, 1, 1), 0),
		  pivot(0.5f, 0.5f),
		  scale(1), rotation(0),
		  color(255,255,255,255)
	{ }

	Sprite::Sprite(Rectangle<float> rect)
		: rect(rect), textureRegion(rect_t(0, 0, 1, 1), 0),
		  pivot(0.5f, 0.5f),
		  zOrder(0), scale(1), rotation(0),
		  color(255,255,255,255)
	{ }

	Sprite::Sprite(Rectangle<float> rect, Resources::TextureRegion texRegion,
				   glm::vec2 pivot,
				   float scale, float rotation, float zOrder,
				   Utils::Color color)
		: rect(rect), textureRegion(texRegion),
		  pivot(pivot),
		  zOrder(zOrder), scale(scale), rotation(rotation),
		  color(color)
	{ }

	Sprite::~Sprite()
	{ }

	void Sprite::fillVertexData(SpriteVertex (&array)[6], Rectangle<float> rect,
								Rectangle<float> texRect, std::uint8_t texLayer, float zOrder, Utils::Color color)
	{
		auto br = rect.getBottomRight();
		auto tr = rect.getTopRight();
		new (&array[0]) SpriteVertex(rect.getBottomRight(), texRect.getBottomRight(), texLayer, zOrder, color);
		new (&array[1]) SpriteVertex(rect.getTopRight(),    texRect.getTopRight(),    texLayer, zOrder, color);
		new (&array[2]) SpriteVertex(rect.getBottomLeft(),  texRect.getBottomLeft(),  texLayer, zOrder, color);

		new (&array[3]) SpriteVertex(rect.getBottomLeft(),  texRect.getBottomLeft(),  texLayer, zOrder, color);
		new (&array[4]) SpriteVertex(rect.getTopRight(),    texRect.getTopRight(),    texLayer, zOrder, color);
		new (&array[5]) SpriteVertex(rect.getTopLeft(),     texRect.getTopLeft(),     texLayer, zOrder, color);
	}
}
