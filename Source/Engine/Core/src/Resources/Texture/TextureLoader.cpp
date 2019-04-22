//
// Created by bentoo on 03.12.16.
//

#include "Core/Resources/Texture/TextureLoader.h"
#include "Core/Resources/Texture/TextureManager.h"
#include "Core/Logger.h"
#include <lodepng.h>

namespace Resources
{
    TextureLoader::TextureLoader(TextureManager& manager,
                                 Memory::IMemoryBlock& memory)
        : _manager(manager), _data() // #NewAlloc
    {

    }

    TextureLoader::~TextureLoader()
    {
        for (auto& data : _data)
        {
            Memory::SafeFree(data.pixels);
        }
    }

    TextureLoader& TextureLoader::loadFromFile(const char* path)
    {
        if (path == nullptr)
            return *this;

        lodepng::State state;

        std::uint8_t*	data;
        std::size_t		size;

        // TODO : use custom file size check function
        // TODO : instead of using lodepngs load_file use their load buffer functions
        auto result = lodepng_load_file(&data, &size, path);

        if (result != 0 || size == 0)
        {
            Core::Logger::warn (
                "TextureLoader : unable to load texture from file '{}', error code '{}', reason '{}'.",
                path, result, lodepng_error_text(result)
            );

            return *this;
        }

        uint32_t imageWidth, imageHeight;

        lodepng_inspect(&imageWidth, &imageHeight, &state, &data[0], size);
        LodePNGColorMode& color = state.info_png.color;

        Core::Logger::info (
            "TextureLoader : BitDepth: '{}', width: '{}', height: '{}'.",
            lodepng_get_bpp(&color), imageWidth, imageHeight
        );

        auto& texture = _data.emplace_back();

        switch (lodepng_get_bpp(&color))
        {
            case 8:
                texture.format = gl::RGBA; // or GL_R, GL_R8, etc.
                texture.pixelFormat = gl::R32F;
                break;
            case 16:
                texture.format = gl::RGBA;
                texture.pixelFormat = gl::RG32F;
                break;
            case 24:
                texture.format = gl::RGBA;
                texture.pixelFormat = gl::RGB32F;
                break;
            case 32:
                texture.format = gl::RGBA;
                texture.pixelFormat = gl::RGBA32F;
                break;
            default:
                Core::Logger::error("TextureLoader : bitdepth '{}' is not supported!", state.info_png.color.bitdepth);
                return *this;
        }

        texture.textureWidth  = imageWidth;
        texture.textureHeight = imageHeight;

        Memory::SafeFree(texture.pixels);

        texture.pixelsSize = lodepng_get_raw_size(texture.textureWidth, texture.textureHeight, &state.info_raw);
        lodepng_decode(&texture.pixels, &texture.textureWidth, &texture.textureHeight, &state, data, size);

        Memory::SafeFree(data);

        Core::Logger::info("TextureLoader : successfully loaded '{}'!", path);

        return *this;
    }

    TextureLoader::handle_t TextureLoader::buildAsSingle()
    {
        YAGE_ASSERT(_data.size() == 1,
                    "TextureLoader : Cannot build as 'single' texture with no or multiple data!");

        if (_data.size() != 1)
            return handle_t::invalid();

        auto& def = _data.front();

        if (def.pixels == nullptr)
            return Texture::handle_t::invalid();

        handle_t textureHandle	= _manager.createTexture();
        Texture& texture		= _manager.getTexture(textureHandle);

        texture._mode			= gl::TEXTURE_2D;
        texture._width			= def.textureWidth;
        texture._height			= def.textureHeight;
        texture._format			= def.format;
        texture._pixelFormat	= def.pixelFormat;

        gl::GenTextures(1, &texture._handle);
        gl::BindTexture(gl::TEXTURE_2D, texture._handle);

        if (OpenGL::checkError())
        {
            Core::Logger::warn (
                    "TextureLoader : Unable to bind texture '{}', due to openGl error!", texture._handle
            );

            gl::BindTexture(gl::TEXTURE_2D, 0);
            return Texture::handle_t::invalid();
        }

        setParameters(gl::TEXTURE_2D, texture);

        /* TODO : Texture MipMaps!
        //if(MipMapLevel != 0)
        {
            gl::TexStorage2D(GL_TEXTURE_2D, MipMapLevel, GL_RGBA8, TextureWidth, TextureHeight);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, TextureWidth, TextureHeight, Format, GL_UNSIGNED_BYTE, &mPixels[0]);
            // glTexImage2D(GL_TEXTURE_2D, 0, PixelFormat, TextureWidth, TextureHeight, 0, Format, GL_UNSIGNED_BYTE, &mPixels[0]);
            glGenerateMipmap(GL_TEXTURE_2D);  //Generate num_mipmaps number of mipmaps here.
        }
        else*/
        {
            gl::BindTexture(gl::TEXTURE_2D, texture._handle);
            gl::TexImage2D (
                gl::TEXTURE_2D, 0, texture._pixelFormat,
                texture._width, texture._height, 0, texture._format,
                gl::UNSIGNED_BYTE, def.pixels
            );
        }

        if (OpenGL::checkError())
        {
            Core::Logger::error (
                "TextureLoader : Unable to create texture '{}' due to openGl error!", texture._handle
            );

            gl::BindTexture(gl::TEXTURE_2D, 0);
            return Texture::handle_t::invalid();
        }

        Core::Logger::info("TextureLoader : Texture '{}' created successfully!", texture._handle);

        gl::BindTexture(gl::TEXTURE_2D, 0);

        return textureHandle;
    }

    bool TextureLoader::validateSameParameters()
    {
        // Require all data to have the same properties
        bool error = false;
        auto& def = _data.front();
        for (auto& data : _data)
        {
            if (data.format != def.format)
            {
                error = true;
                Core::Logger::error("TextureLoader : unable to load array texture with different formats!");
            }

            if (data.pixelFormat != def.pixelFormat)
            {
                error = true;
                Core::Logger::error("TextureLoader : unable to load array texture with different pixel formats!");
            }

            if (data.pixelsSize != def.pixelsSize)
            {
                error = true;
                Core::Logger::error("TextureLoader : unable to load array texture with different pixel sizes!");
            }

            if (data.textureHeight != def.textureHeight)
            {
                error = true;
                Core::Logger::error("TextureLoader : unable to load array texture with different heights!");
            }

            if (data.textureWidth != def.textureWidth)
            {
                error = true;
                Core::Logger::error("TextureLoader : unable to load array texture with different widths!");
            }

            if (data.pixels == nullptr)
            {
                error = true;
                Core::Logger::error("TextureLoader : unable to load array texture with empty data!");
            }
        }

        return !error;
    }

    TextureLoader::handle_t TextureLoader::buildAsMulti(MultiTextureType::Enum type)
    {
        YAGE_ASSERT(_data.size() > 0,
                    "TextureLoader : Cannot build texture with no data!");

        if (_data.size() == 0)
            return handle_t::invalid();

        if (type == MultiTextureType::Texture2DArray)
        {
            if (!validateSameParameters())
                return handle_t::invalid();
        }

        auto& def = _data.front();

        handle_t textureHandle	= _manager.createTexture();
        Texture& texture		= _manager.getTexture(textureHandle);

        texture._mode			= gl::TEXTURE_2D_ARRAY;
        texture._width			= def.textureWidth;
        texture._height			= def.textureHeight;
        texture._format			= def.format;
        texture._pixelFormat	= def.pixelFormat;

        gl::GenTextures(1, &texture._handle);
        gl::BindTexture(gl::TEXTURE_2D_ARRAY, texture._handle);

        if (OpenGL::checkError())
        {
            Core::Logger::warn (
                "TextureLoader : Unable to bind texture '{}', due to openGl error!", texture._handle
            );

            gl::BindTexture(gl::TEXTURE_2D_ARRAY, 0);
            return Texture::handle_t::invalid();
        }

        gl::TexImage3D(gl::TEXTURE_2D_ARRAY, 0,
                       def.pixelFormat, def.textureWidth, def.textureHeight,
                       _data.size(), 0, def.format, gl::UNSIGNED_BYTE, nullptr);

        if (OpenGL::checkError())
        {
            Core::Logger::error (
                "TextureLoader : Unable to create texture '{}' due to openGl error!", texture._handle
            );

            gl::BindTexture(gl::TEXTURE_2D_ARRAY, 1);
            gl::DeleteTextures(1, &texture._handle);
            return Texture::handle_t::invalid();
        }

        int zOffset = 0;
        for (auto& data : _data)
        {
            gl::TexSubImage3D(gl::TEXTURE_2D_ARRAY, 0, 0, 0, zOffset,
                              data.textureWidth, data.textureHeight, 1,
                              def.format, gl::UNSIGNED_BYTE, def.pixels);

            if (OpenGL::checkError())
            {
                Core::Logger::error (
                    "TextureLoader : Unable to create texture '{}' due to openGl error!", texture._handle
                );

                gl::BindTexture(gl::TEXTURE_2D_ARRAY, 0);
                gl::DeleteTextures(1, &texture._handle);
                return Texture::handle_t::invalid();
            }

            zOffset++;
        }

        gl::GenerateMipmap(gl::TEXTURE_2D_ARRAY);

        gl::TexParameteri(gl::TEXTURE_2D_ARRAY, gl::TEXTURE_BASE_LEVEL, 0);
        gl::TexParameteri(gl::TEXTURE_2D_ARRAY, gl::TEXTURE_MAX_LEVEL,  0);

        setParameters(gl::TEXTURE_2D_ARRAY, texture);

        if (!OpenGL::checkError())
            Core::Logger::info("TextureLoader : Texture '{}' created successfully!", texture._handle);

        gl::BindTexture(gl::TEXTURE_2D_ARRAY, 0);

        return textureHandle;
    }

    void TextureLoader::setParameters(GLenum slot, const Texture &texture) const
    {
        gl::BindTexture(slot, texture._handle);

        for (auto& pair : parameters)
        {
            gl::TexParameteri(slot, pair.first, pair.second);
            if (OpenGL::checkError())
            {
                Core::Logger::warn (
                    "TextureLoader : Unable to set parameter '{}' as '{}' due to openGL error!",
                    pair.first, pair.second
                );
            }
        }

        gl::BindTexture(slot, 0);
        OpenGL::checkError();
    }

    TextureLoader& TextureLoader::setParameter(GLenum key, GLenum value)
    {
        parameters[key] = value;
        return *this;
    }
}
