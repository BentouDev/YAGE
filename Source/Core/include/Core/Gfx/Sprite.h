//
// Created by bentoo on 16.02.17.
//

#ifndef YAGE_SPRITE_H
#define YAGE_SPRITE_H

#include <Utils/Color.hpp>
#include <glm/vec2.hpp>
#include "Rectangle.h"

namespace Gfx
{
	struct SpriteVertex
	{
		SpriteVertex()
			: position(0, 0), textureCoordinate(0, 0), color(255, 255, 255, 255)
		{ }

		inline SpriteVertex(glm::vec2 pos, glm::vec2 tex, Utils::Color col)
			: position(pos), textureCoordinate(tex), color(col)
		{ }

		glm::vec2		position;
		glm::vec2		textureCoordinate;
		Utils::Color	color;
	};

	class Sprite
	{
		Rectangle<float>	rect;
		Rectangle<float>	texRect;
		glm::vec2			pivot;
		float				scale;
		float				rotation;
		Utils::Color		color;

	public:
				 Sprite();
		explicit Sprite(Rectangle<float> rect);
				 Sprite(Rectangle<float> rect, Rectangle<float> texRect, glm::vec2 pivot = glm::vec2(0.5f, 0.5f),
						float scale = 1, float rotation = 0, Utils::Color color = Utils::Color(255,255,255,255));
		virtual ~Sprite();

		const Rectangle<float>& getRect() const { return rect; }
		glm::vec2 getPivot() const { return pivot; }
		float getScale() const { return scale; }
		float getRotation() const { return rotation; }

		inline void fillVertexData(SpriteVertex (&array)[6]) const { fillVertexData(array, rect, texRect, color); }
		static void fillVertexData(SpriteVertex (&array)[6], Rectangle<float> rect,
								   Rectangle<float> texRect, Utils::Color color);
	};
}

#endif //YAGE_SPRITE_H
