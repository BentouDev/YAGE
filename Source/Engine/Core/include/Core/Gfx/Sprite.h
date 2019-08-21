//
// Created by bentoo on 16.02.17.
//

#ifndef YAGE_SPRITE_H
#define YAGE_SPRITE_H

#include <Utils/Color.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "Core/Resources/Texture/TextureRegion.h"

namespace Gfx
{
	struct SpriteVertex
	{
		SpriteVertex()
			: position(0, 0, 0), textureCoordinate(0, 0), color(255, 255, 255, 255)
		{ }

		inline SpriteVertex(glm::vec2 pos, glm::vec2 tex, std::uint8_t texLayer, float zOrder, Utils::Color col)
			: position(pos, zOrder), textureCoordinate(tex), textureLayer(texLayer), color(col)
		{ }

		glm::vec3		position;
		glm::vec2		textureCoordinate;
		std::uint8_t 	textureLayer;
		Utils::Color	color;
	};

	class Sprite
	{
		Rectangle<float>			rect;
		Resources::TextureRegion	textureRegion;

		glm::vec2			pivot;
		float 				zOrder;
		float				scale;
		float				rotation;
		Utils::Color		color;

	public:
        static constexpr std::uint32_t VERTEX_COUNT = 6;
        using TVertexArray = SpriteVertex[Sprite::VERTEX_COUNT];

				 Sprite();
		explicit Sprite(Rectangle<float> rect);
				 Sprite(Rectangle<float> rect, Resources::TextureRegion texRegion,
						glm::vec2 pivot = glm::vec2(0.5f, 0.5f),
						float scale = 1, float rotation = 0, float zOrder = 0,
						Utils::Color color = Utils::Color(255,255,255,255));
		virtual ~Sprite();

		const Rectangle<float>& getRect() const { return rect; }
		glm::vec2 getPivot() const { return pivot; }
		float getScale() const { return scale; }
		float getRotation() const { return rotation; }

		inline void fillVertexData(TVertexArray& array) const { fillVertexData(array, rect, textureRegion.coordinates,
																					textureRegion.layer, zOrder, color); }
		static void fillVertexData(TVertexArray& array, Rectangle<float> rect,
								   Rectangle<float> texRect, std::uint8_t texLayer, float zOrder, Utils::Color color);
	};
}

#endif //YAGE_SPRITE_H
