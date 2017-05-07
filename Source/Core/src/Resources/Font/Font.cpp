//
// Created by Bentoo on 2017-04-16.
//

#include "Core/Resources/Font/Font.h"

namespace Resources
{
	Font::Font(Memory::IMemoryBlock& memory)
		: charset(memory), textures(memory),
		  size(1,1), lineHeight(0), base(0),
		  width(0), height(0),
		  firstChar(0), lastChar(0)
	{ }

	Font::Font(Font &&other)
		: charset(std::move(other.charset)),
		  textures(std::move(other.textures)),
		  size(other.size), lineHeight(other.lineHeight), base(other.base),
		  width(other.width), height(other.height),
		  firstChar(other.firstChar), lastChar(other.lastChar)
	{ }

	Font::~Font()
	{

	}

	CharData* Font::lookupChar(wchar_t code)
	{
		auto itr = charLookup.find(code);
		if (itr != charLookup.end())
		{
			return &charset[itr->second];
		}

		return nullptr;
	}
}
