//
// Created by Bentoo on 2017-04-16.
//

#ifndef YAGE_FONTLOADER_H
#define YAGE_FONTLOADER_H

#include "Core/Resources/ObjParser/ObjParser.h"
#include <Utils/MemoryBlock.h>
#include <Utils/List.h>
#include <Utils/String.h>
#include <Utils/Handle.h>

namespace Core
{
	class Engine;
}

namespace Resources
{
	class Font;
	class FontManager;
	class TextureLoader;

	class FontLoader : protected ObjParser<Font, FontLoader>
	{
		Resources::TextureLoader*	textureLoader;
		Resources::FontManager&	 	_manager;
		Memory::IMemoryBlock&		_memory;
		Utils::List<Utils::String>	textures;

		const char* filepath;

		static bool parseInfo	(std::string line, FontLoader& loader, Font& font);
		static bool parseCommon	(std::string line, FontLoader& loader, Font& font);
		static bool parsePage	(std::string line, FontLoader& loader, Font& font);
		static bool parseChars	(std::string line, FontLoader& loader, Font& font);
		static bool parseChar	(std::string line, FontLoader& loader, Font& font);

	public:
		using handle_t = Utils::Handle<Font>;

		explicit FontLoader(Resources::FontManager& manager, Memory::IMemoryBlock& memory);
		virtual ~FontLoader();

		FontLoader& withTextureLoader(Resources::TextureLoader& loader);
		FontLoader& loadFromFile(const char* path);
		handle_t	build();
	};
}

#endif //YAGE_FONTLOADER_H
