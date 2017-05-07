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
		: _memory(memory), text(memory)
	{

	}

	TextBuilder::~TextBuilder()
	{

	}

	Utils::String& TextBuilder::getString()
	{
		return text;
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

	void TextBuilder::drawAsSpriteBatch(Renderer& renderer, Gfx::Camera& camera)
	{
		auto& batch = renderer.getSpriteBatch(material, &camera);

		Utils::List<Utils::Slice<char>> tokens(_memory);
		Utils::String::Tokenize(text, tokens, "\n\r");

		for (auto& line : tokens)
		{
			drawTextInternal(line, batch);
		}
	}

	int TextBuilder::lineLength(Utils::Slice<char> line)
	{
		if (font == nullptr)
			return 0;

		int length = 0;
		auto itr = line.begin();
		do {
			auto  code     = utf8::next(itr, line.end());
			auto* charData = font->lookupChar(code);

			length += charData != nullptr ? charData->advance : 0;

		} while(itr != line.end());

		return length;
	}

	void TextBuilder::drawTextInternal(Utils::Slice<char> line, Gfx::SpriteBatch &batch)
	{
		if (!utf8::is_valid(line.begin(), line.end()))
			return;

		auto count  = utf8::distance(line.begin(), line.end());
		auto length = lineLength(line);
		auto itr    = line.begin();

		float currentX = 0;
		float currentY = 0;

		do {
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

			batch.drawSprite(position, texcoord, 1, 0, color);
		} while(itr != line.end());
	}
}
