//
// Created by bentoo on 01.12.16.
//

#ifndef GAME_TEXTURE_H
#define GAME_TEXTURE_H

#include "Core/Resources/Resource.h"
#include "Core/Gfx/OpenGl/OpenGLBase.h"

namespace Meta
{
	class RegisterClass;
}

namespace Resources
{
	YClass(Serialize())
    class Texture : public Core::IResource
    {
		friend class Meta::RegisterClass;
        friend class TextureLoader;

        uint32_t _width;
        uint32_t _height;

        GLuint _handle;
        GLenum _mode;
        GLenum _format;
        GLenum _pixelFormat;

    public:
        explicit Texture();
        Texture(Texture&& other) noexcept;

        Texture(const Texture&) = delete;
        Texture& operator=(Texture&&) = delete;
        Texture& operator=(const Texture&) = delete;

        virtual ~Texture();

        GLenum getMode() const
        { return _mode; }

        inline operator GLuint() const
        { return _handle; }
    };
}

#endif //GAME_TEXTURE_H
