//
// Created by Bentoo on 2017-04-16.
//

#include "Core/Resources/Font/FontLoader.h"
#include "Core/Resources/Font/FontManager.h"
#include "Core/Resources/Texture/TextureLoader.h"
#include <Utils/String.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

namespace Resources
{
    FontLoader::FontLoader(Resources::FontManager& manager, Memory::IMemoryBlock& memory)
        : _manager(manager), _memory(memory), textureLoader(nullptr), filepath(nullptr), textures() // #NewAlloc
    {
        lookupMap["info"]	= &parseInfo;
        lookupMap["common"]	= &parseCommon;
        lookupMap["page"]	= &parsePage;
        lookupMap["chars"]	= &parseChars;
        lookupMap["char"]	= &parseChar;
    }

    FontLoader::~FontLoader()
    {

    }

    FontLoader& FontLoader::withTextureLoader(Resources::TextureLoader& loader)
    {
        textureLoader = &loader;
        return *this;
    }

    FontLoader& FontLoader::loadFromFile(const char *path)
    {
        if(path != nullptr)
            filepath = path;

        return *this;
    }

    FontLoader::handle_t FontLoader::build()
    {
        std::fstream	file(filepath, std::fstream::in);
        std::string		line;

        auto font =  _manager.createFont();

        while (std::getline(file, line))
        {
            ProcessLine(line, *this, *font);
        }

        // Process loaded textures and merge them into texture array
        for (auto& texturePaths : textures)
        {
            textureLoader->loadFromFile(texturePaths.c_str());
        }

        font->textureAtlas = textureLoader->buildAsMulti(MultiTextureType::Texture2DArray);

        return font;
    }

    bool FontLoader::parseInfo(std::string line, FontLoader &loader, Font &font)
    {
        std::istringstream stream(line);
        std::vector<std::string> tokens {std::istream_iterator<std::string>(stream),
                                         std::istream_iterator<std::string>()};

        for(auto& token : tokens)
        {
            std::stringstream converter;

            auto eq_pos	= token.find('=');

            if (eq_pos == std::string::npos)
                continue;

            auto key	= token.substr(0, eq_pos);
            auto value	= token.substr(eq_pos + 1, token.size() - 2);

            converter << value;

            // Todo : refactor to std::map or something...
            if(strcmp(key.c_str(), "face") == 0)
                converter >> font.Name;
        }

        return true;
    }

    bool FontLoader::parseCommon(std::string line, FontLoader &loader, Font &font)
    {
        std::istringstream stream(line);
        std::vector<std::string> tokens {std::istream_iterator<std::string>(stream),
                                         std::istream_iterator<std::string>()};

        for(auto& token : tokens)
        {
            std::stringstream converter;

            auto eq_pos	= token.find('=');
            auto key	= token.substr(0, eq_pos);
            auto value	= token.substr(eq_pos + 1, token.size() - 2);

            converter << value;

            // Todo : refactor to std::map or something...
            if(strcmp(key.c_str(), "lineHeight") == 0)
                converter >> font.lineHeight;
            else if(strcmp(key.c_str(), "base") == 0)
                converter >> font.base;
            else if(strcmp(key.c_str(), "scaleW") == 0)
                converter >> font.width;
            else if(strcmp(key.c_str(), "scaleH") == 0)
                converter >> font.height;
            else if(strcmp(key.c_str(), "pages") == 0)
            {
                int pageCount = 0;
                converter >> pageCount;
                loader.textures.reserve(pageCount);
            }
        }

        return true;
    }

    bool FontLoader::parsePage(std::string line, FontLoader &loader, Font &font)
    {
        std::istringstream stream(line);
        std::vector<std::string> tokens {std::istream_iterator<std::string>(stream),
                                         std::istream_iterator<std::string>()};

        for(auto& token : tokens)
        {
            std::stringstream converter;

            auto eq_pos	= token.find('=');
            auto key	= token.substr(0, eq_pos);
            auto value	= token.substr(eq_pos + 1, token.size() - 2);

            converter << value;

            // Todo : refactor to std::map or something...
            if(strcmp(key.c_str(), "file") == 0)
            {
                if(loader.textureLoader != nullptr)
                {
                    std::string filePath(loader.filepath);
                    std::string path = filePath.substr(0, filePath.find_last_of('/') + 1);
                                path += value.substr(1, value.size() - 2);
                    loader.textures.emplace_back(path.c_str()); // #NewAlloc loader._memory
                }
            }
        }

        return true;
    }

    bool FontLoader::parseChars(std::string line, FontLoader &loader, Font &font)
    {
        std::istringstream stream(line);
        std::vector<std::string> tokens {std::istream_iterator<std::string>(stream),
                                         std::istream_iterator<std::string>()};

        for(auto& token : tokens)
        {
            std::stringstream converter;

            auto eq_pos	= token.find('=');
            auto key	= token.substr(0, eq_pos);
            auto value	= token.substr(eq_pos + 1, token.size() - 2);

            converter << value;

            // Todo : refactor to std::map or something...
            if(strcmp(key.c_str(), "count") == 0)
            {
                int count = 0;
                converter >> count;
                font.charset.reserve(count);
            }
        }

        return true;
    }

    bool FontLoader::parseChar(std::string line, FontLoader &loader, Font &font)
    {
        YAGE_ASSERT(font.charset.capacity() > 0, "Charset memory must have been alocated previously!");

        std::istringstream stream(line);
        std::vector<std::string> tokens {std::istream_iterator<std::string>(stream),
                                         std::istream_iterator<std::string>()};

        for (auto& token : tokens)
        {
            std::stringstream converter;

            auto eq_pos	= token.find('=');
            auto key	= token.substr(0, eq_pos);
            auto value	= token.substr(eq_pos + 1, token.size() - 2);

            converter << value;

            //if(font.charset.size() == 0)
            //	font.charset.emplace();

            // Todo : refactor to std::map or something...
            if (strcmp(key.c_str(), "id") == 0)
            {
                int charId = 0;
                converter >> charId;

                font.charLookup[charId] = font.charset.size();

                if (font.charset.size() == 0)
                    font.firstChar = charId;

                font.charset.emplace_back();
            }
            else if (strcmp(key.c_str(), "x") == 0)
            {
                float x = 0;
                converter >> x;
                font.charset.back().textureRect.setMinX(x);
            }
            else if (strcmp(key.c_str(), "y") == 0)
            {
                float y = 0;
                converter >> y;
                font.charset.back().textureRect.setMinY(y);
            }
            else if (strcmp(key.c_str(), "width") == 0)
            {
                float w = 0;
                converter >> w;
                font.charset.back().textureRect.setWidth(w);
            }
            else if (strcmp(key.c_str(), "height") == 0)
            {
                float h = 0;
                converter >> h;
                font.charset.back().textureRect.setHeight(h);
            }
            else if (strcmp(key.c_str(), "xoffset") == 0)
            {
                float x = 0;
                converter >> x;
                font.charset.back().offset.x = x;
            }
            else if (strcmp(key.c_str(), "yoffset") == 0)
            {
                float y = 0;
                converter >> y;
                font.charset.back().offset.y = y;
            }
            else if (strcmp(key.c_str(), "xadvance") == 0)
            {
                std::uint32_t x = 0;
                converter >> x;
                font.charset.back().advance = x;
            }
            else if (strcmp(key.c_str(), "page") == 0)
            {
                std::uint32_t page = 0;
                converter >> page;
                font.charset.back().page = page;
            }
        }

        return true;
    }
}
