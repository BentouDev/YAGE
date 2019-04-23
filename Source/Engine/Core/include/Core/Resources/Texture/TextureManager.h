//
// Created by bentoo on 03.12.16.
//

#ifndef GAME_TEXTUREMANAGER_H
#define GAME_TEXTUREMANAGER_H

#include <Utils/SmartHandle.h>
#include "Core/IManager.h"
#include "Texture.h"

namespace Resources
{
    class Texture;

    class TextureManager : public Core::IManager
    {
        MANAGER(TextureManager);

    public:
        using handle_t = Utils::Handle<Texture>;

    protected:
        Utils::Colony<Texture> _textureContainer;

        void disposeTexture(Texture& texture);

    public:
        explicit TextureManager(Core::Engine &engine, Memory::IMemoryBlock &memory);
        virtual ~TextureManager();

        TextureManager(TextureManager&&) = delete;
        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(TextureManager&&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;

        void disposeTexture(handle_t texture);

        Utils::SmartHandle<Resources::Texture> toSmartHandle(handle_t handle) const;

        Texture* tryGetTexture(handle_t);

        bool hasTexture(handle_t handle)
        { return _textureContainer.contains(handle); }

        handle_t createTexture()
        { return _textureContainer.emplace(); }

        Texture& getTexture(handle_t handle)
        { return _textureContainer.get(handle); }

        const Texture& getTexture(handle_t handle) const
        { return _textureContainer.get(handle); }
    };
}

#endif //GAME_TEXTUREMANAGER_H
