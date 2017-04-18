//
// Created by Bentoo on 2017-04-16.
//

#ifndef YAGE_FONTMANAGER_H
#define YAGE_FONTMANAGER_H

#include "Core/IManager.h"
#include "Core/Engine.h"
#include "Core/Resources/Font/Font.h"

#include <Utils/MemoryBlock.h>
#include <Utils/Container.h>

namespace Resources
{
	class FontManager : public Core::IManager
	{
		MANAGER(FontManager);

	protected:
		Utils::Container<Font::trait_t> _fontContainer;

	public:
		using handle_t = Font::handle_t;

		explicit FontManager(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~FontManager();

		FontManager(FontManager&&) = delete;
		FontManager(const FontManager&) = delete;
		FontManager& operator=(FontManager&&) = delete;
		FontManager& operator=(const FontManager&) = delete;

		Font* tryGetFont(handle_t);

		inline bool hasFont(handle_t handle)
		{ return _fontContainer.contains(handle); }

		inline handle_t createFont()
		{ return _fontContainer.create(_memory); }

		inline Font& getFont(handle_t handle)
		{ return _fontContainer.get(handle); }
	};
}

#endif //YAGE_FONTMANAGER_H
