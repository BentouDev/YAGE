//
// Created by Bentoo on 2017-04-16.
//

#include "Core/Resources/Font/FontManager.h"

namespace Resources
{
    FontManager::FontManager(Core::Engine &engine, Memory::IMemoryBlock &memory)
        : Core::IManager(engine, memory), _fontContainer(_memory)
    { }

    FontManager::~FontManager()
    { }

    Font* FontManager::tryGetFont(handle_t handle)
    {
        if(_fontContainer.contains(handle))
            return &_fontContainer.get(handle);

        return nullptr;
    }

    FontManager::smart_t FontManager::createFont()
    {
        return Utils::make_handle<FontTrait>(&_fontContainer, _memory);
    }
}
