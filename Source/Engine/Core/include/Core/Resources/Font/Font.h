//
// Created by Bentoo on 2017-04-16.
//

#ifndef YAGE_FONT_H
#define YAGE_FONT_H

#include <map>
#include <Utils/List.h>
#include <Utils/MemoryBlock.h>

#include <Platform/Graphics/Rectangle.h>

#include "Core/Resources/Resource.h"

// temporary
#include "Core/Resources/Texture/Texture.h"

namespace Meta
{
    class RegisterClass;
}

namespace Resources
{
	class Texture;

    struct CharData
    {
        CharData()
            : textureRect(0,0,1,1), offset(0,0), advance(1), page(0)
        { }

        Gfx::Rectangle<float>	textureRect;
        glm::vec2				offset;
        std::uint32_t			advance;
        std::uint8_t			page;
    };

    YClass(Serialize());
    class Font : public Core::IResource
    {
        friend class Meta::RegisterClass;

        friend class FontLoader;

        Utils::Handle<Texture>  			textureAtlas;
        Utils::List<CharData>				charset;
        std::map<wchar_t, std::uint32_t>	charLookup;

        glm::vec2		size;

        unsigned short	lineHeight;
        unsigned short	base;
        unsigned short	width;
        unsigned short	height;
        unsigned int	firstChar;
        unsigned int	lastChar;

    public:
        explicit Font(/*Memory::IMemoryBlock& memory*/);
        virtual ~Font();

        Font(Font&& other);

        CharData* lookupChar(wchar_t code);
		Utils::Handle<Texture> getTextureAtlas()
        { return textureAtlas; }

        unsigned short getWidth() const { return width; }
        unsigned short getHeight() const { return height; }
        unsigned short getLineHeight() const { return lineHeight; }

        Font(const Font&) = delete;
        Font& operator=(Font&&) = delete;
        Font& operator=(const Font&) = delete;
    };
}

#include "Font.gen.h"

#endif //YAGE_FONT_H
