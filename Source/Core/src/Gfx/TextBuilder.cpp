//
// Created by bentoo on 18.04.17.
//

#include <Utils/Slice.h>
#include "Core/Gfx/TextBuilder.h"
#include "Core/Gfx/Renderer.h"

namespace Gfx
{
	TextBuilder::TextBuilder(Memory::IMemoryBlock &memory)
		: _memory(memory), text(memory)
	{

	}

	TextBuilder::~TextBuilder()
	{

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

	void TextBuilder::drawTextInternal(Utils::Slice<char> line, Gfx::SpriteBatch &batch)
	{

	}
}
