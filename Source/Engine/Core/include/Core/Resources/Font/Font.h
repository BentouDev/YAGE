//
// Created by Bentoo on 2017-04-16.
//

#ifndef YAGE_FONT_H
#define YAGE_FONT_H

#include <map>
#include <Utils/List.h>
#include <Utils/DefaultTrait.h>
#include <Utils/MemoryBlock.h>

#include "Core/Resources/Resource.h"
#include "Core/Resources/Texture/Texture.h"
#include "Core/Gfx/Rectangle.h"

namespace Resources
{
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

    DECL_RESOURCE(Font)
    {
        friend class FontLoader;

        Texture::handle_t					textureAtlas;
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
        explicit Font(Memory::IMemoryBlock& memory);
        virtual ~Font();

        Font(Font&& other);

        CharData* lookupChar(wchar_t code);
        Texture::handle_t getTextureAtlas()
        { return textureAtlas; }

        unsigned short getWidth() const { return width; }
        unsigned short getHeight() const { return height; }
        unsigned short getLineHeight() const { return lineHeight; }

        Font(const Font&) = delete;
        Font& operator=(Font&&) = delete;
        Font& operator=(const Font&) = delete;
    };

    class FontTrait : public Utils::DefaultTrait<Font>
    { };
}

#endif //YAGE_FONT_H
