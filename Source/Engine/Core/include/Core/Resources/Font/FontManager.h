//
// Created by Bentoo on 2017-04-16.
//

#ifndef YAGE_FONTMANAGER_H
#define YAGE_FONTMANAGER_H

#include "Core/IManager.h"
#include "Core/Engine.h"
#include "Core/Resources/Font/Font.h"

#include <Utils/MemoryBlock.h>
#include <Utils/SmartHandle.h>
#include <Utils/Container.h>

namespace Resources
{
    class FontManager : public Core::IManager
    {
        MANAGER(FontManager);

    protected:
        Utils::Container<FontTrait> _fontContainer;

    public:
        using handle_t = Font::handle_t;
        using smart_t  = Utils::SmartHandle<FontTrait>;

        explicit FontManager(Core::Engine& engine, Memory::IMemoryBlock& memory);
        virtual ~FontManager();

        FontManager(FontManager&&) = delete;
        FontManager(const FontManager&) = delete;
        FontManager& operator=(FontManager&&) = delete;
        FontManager& operator=(const FontManager&) = delete;

        smart_t createFont();

        Font* tryGetFont(handle_t);

        inline bool hasFont(handle_t handle)
        { return _fontContainer.contains(handle); }

        inline Font& getFont(handle_t handle)
        { return _fontContainer.get(handle); }
    };
}

#endif //YAGE_FONTMANAGER_H
