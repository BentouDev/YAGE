//
// Created by bentoo on 03.12.16.
//

#include "Core/Resources/Texture/TextureManager.h"

namespace Resources
{
    TextureManager::TextureManager(Core::Engine &engine, Memory::IMemoryBlock &memory)
        : IManager(engine, memory), _textureContainer(_memory)
    {

    }

    TextureManager::~TextureManager()
    {

    }

    void TextureManager::disposeTexture(handle_t texture)
    {

    }

    void TextureManager::disposeTexture(Texture &texture)
    {

    }

    Texture* TextureManager::tryGetTexture(handle_t handle)
    {
        Texture* result = nullptr;

        if(hasTexture(handle))
        {
            result = &getTexture(handle);
        }

        return result;
    }

    Utils::SmartHandle<Resources::Texture::trait_t> TextureManager::toSmartHandle(
            Resources::TextureManager::handle_t handle) const
    {
        return Utils::SmartHandle<Resources::Texture::trait_t>(const_cast<Utils::Colony<Resources::Texture::trait_t>*>(&_textureContainer), handle);
    }
}
