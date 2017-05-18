//
// Created by bentoo on 18.04.17.
//

#include <utf8.h>
#include <Utils/Slice.h>
#include "Core/Resources/Font/Font.h"
#include "Core/Gfx/TextBuilder.h"
#include "Core/Gfx/Renderer.h"
#include "Core/Gfx/SpriteBatch.h"

namespace Gfx
{
	TextBuilder::TextBuilder(Memory::IMemoryBlock &memory)
		: _memory(memory), text(memory), posOffset(0, 0), zOrder(0)
	{

	}

	TextBuilder::~TextBuilder()
	{

	}

	Utils::String& TextBuilder::getString()
	{
		return text;
	}

	Gfx::SpriteBatch& TextBuilder::getBatch(Renderer& renderer, Gfx::Camera& camera)
	{
		return renderer.getSpriteBatch(material, &camera);
	}

	TextBuilder& TextBuilder::clearText()
	{
		text.clear();
		return *this;
	}

	TextBuilder& TextBuilder::appendText(const char* string)
	{
		text.append(string);
		return *this;
	}

	TextBuilder& TextBuilder::withFont(Resources::Font& font)
	{
		this->font = &font;
		return *this;
	}

	TextBuilder& TextBuilder::withMaterial(Utils::Handle<Core::Material> material)
	{
		this->material = material;
		return *this;
	}

	TextBuilder& TextBuilder::horizontalAlign(HorizontalAlign::Type align)
	{
		horizontal = align;
		return *this;
	}

	TextBuilder& TextBuilder::verticalAlign(VerticalAlign::Type align)
	{
		vertical = align;
		return *this;
	}

	TextBuilder& TextBuilder::withOffset(glm::vec2 offset)
	{
		posOffset = offset;
		return *this;
	}

	TextBuilder& TextBuilder::withZOrder(float zOrder)
	{
		this->zOrder = zOrder;
		return *this;
	}

	void TextBuilder::drawAsSpriteBatch(Renderer& renderer, Gfx::Camera& camera)
	{
		auto& batch = renderer.getSpriteBatch(material, &camera);

		Utils::List<Utils::Slice<char>> lines(_memory);
		Utils::String::Tokenize(text, lines, "\n\r");

		glm::vec2 offset = posOffset;
		glm::vec2 lineHeight(0, font->getLineHeight() / (float)font->getHeight());

		switch (vertical)
		{
			case VerticalAlign::Bottom :
				offset -= lineHeight * (float) lines.size();
				break;
			case VerticalAlign::Center :
				offset -= lineHeight * (float) lines.size() * 0.5f;
				break;
			default:
				break;
		}

		for (std::size_t i = 0; i < lines.size(); i++)
		{
			drawTextInternal(lines[i], batch, (lineHeight * (float)i) + offset);
		}
	}

	int TextBuilder::lineLength(Utils::Slice<char> line)
	{
		if (font == nullptr)
			return 0;

		int length = 0;
		auto itr = line.begin();
		do
		{
			auto  code     = utf8::next(itr, line.end());
			auto* charData = font->lookupChar(code);

			length += charData != nullptr ? charData->advance : 0;

		} while (itr != line.end());

		return length;
	}

	void TextBuilder::drawTextInternal(Utils::Slice<char> line, Gfx::SpriteBatch &batch, glm::vec2 offset)
	{
		if (!utf8::is_valid(line.begin(), line.end()))
			return;

		auto length = lineLength(line);
		auto itr    = line.begin();

		float currentX = offset.x;
		float currentY = offset.y;

		switch (horizontal)
		{
			case HorizontalAlign::Right:
				currentX -= length / (float) font->getWidth();
				break;
			case HorizontalAlign::Center:
				currentX -= 0.5f * length / (float) font->getWidth();
				break;
			default:
				break;
		}

		do
		{
			auto* charData = font->lookupChar(utf8::next(itr, line.end()));

			if (charData == nullptr)
				continue;

			Gfx::Rectangle<float> position (
				currentX + (charData->offset.x / font->getWidth()),
				currentY + (charData->offset.y / font->getHeight()),
				charData->textureRect.getWidth() / font->getWidth(),
				charData->textureRect.getHeight() / font->getHeight()
			);

			Gfx::Rectangle<float> texcoord (
				charData->textureRect.getLeft() / font->getWidth(),
				charData->textureRect.getBottom() / font->getHeight(),
				charData->textureRect.getWidth() / font->getWidth(),
				charData->textureRect.getHeight() / font->getHeight()
			);

			currentX += charData->advance / (float) font->getWidth();

			batch.drawSprite(position, glm::vec2(0.5f, 0.5f), texcoord, charData->page, 1, 0, zOrder, color);
		} while (itr != line.end());
	}
}
