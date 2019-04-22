//
// Created by bentoo on 18.04.17.
//

#ifndef YAGE_TEXT_H
#define YAGE_TEXT_H

#include <Utils/String.h>
#include <Utils/Handle.h>
#include <Utils/Color.hpp>
#include <Utils/MemoryBlock.h>

namespace Core
{
	class Material;
}

namespace Resources
{
	class Font;
}

namespace Gfx
{
	class Camera;
	class Renderer;
	class SpriteBatch;

	namespace VerticalAlign
	{
		enum Type
		{
			Top,
			Center,
			Bottom
		};
	}

	namespace HorizontalAlign
	{
		enum Type
		{
			Left,
			Center,
			Right
		};
	}

	class TextBuilder
	{
		Memory::IMemoryBlock&			_memory;
		Utils::String					text;
		Resources::Font*				font;
		Utils::Color					color;
		Utils::Handle<Core::Material>	material;
		glm::vec2						posOffset;
		float 							zOrder;
		HorizontalAlign::Type 			horizontal;
		VerticalAlign::Type 			vertical;

		void drawTextInternal(Utils::Slice<char> line, Gfx::SpriteBatch& batch, glm::vec2 offset);
		int lineLength(Utils::Slice<char> line);

	public:
		explicit TextBuilder(Memory::IMemoryBlock& memory);
		virtual ~TextBuilder();

		Utils::String&		getString();
		Gfx::SpriteBatch&	getBatch(Renderer& renderer, Gfx::Camera& camera);

		TextBuilder& clearText();
		TextBuilder& appendText(const char* string);
		TextBuilder& withFont(Resources::Font& font);
		TextBuilder& withMaterial(Utils::Handle<Core::Material> material);
		TextBuilder& withOffset(glm::vec2 offset);
		TextBuilder& withZOrder(float zOrder);
		TextBuilder& horizontalAlign(HorizontalAlign::Type align);
		TextBuilder& verticalAlign(VerticalAlign::Type align);
		void 		 drawAsSpriteBatch(Renderer& renderer, Gfx::Camera& camera);
	};

	inline static TextBuilder& operator<<(TextBuilder& str, char c)
	{
		str.getString() += c;
		return str;
	}

	inline static TextBuilder& operator<<(TextBuilder& str, const char *s)
	{
		str.getString() += s;
		return str;
	}

	inline static TextBuilder& operator<<(TextBuilder& str, float f)
	{
		str.getString() += f;
		return str;
	}

	inline static TextBuilder& operator<<(TextBuilder& str, int32_t i)
	{
		str.getString() += i;
		return str;
	}

	inline static TextBuilder& operator<<(TextBuilder& str, uint32_t u)
	{
		str.getString() += u;
		return str;
	}

	inline static TextBuilder& operator<<(TextBuilder& str, uint64_t i)
	{
		str.getString() += i;
		return str;
	}
}

#endif //YAGE_TEXT_H
