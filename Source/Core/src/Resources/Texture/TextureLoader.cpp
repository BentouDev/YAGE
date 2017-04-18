//
// Created by bentoo on 03.12.16.
//

#include "Core/Resources/Texture/TextureLoader.h"
#include "Core/Resources/Texture/TextureManager.h"
#include "Core/Logger.h"
#include <lodepng.h>

namespace Resources
{
	TextureLoader::TextureLoader(TextureManager& manager)
		: _manager(manager), pixels(nullptr),
		  textureWidth(0), textureHeight(0)
	{

	}

	TextureLoader::~TextureLoader()
	{
		Memory::SafeFree(pixels);
	}

	TextureLoader& TextureLoader::setWidth(uint32_t width)
	{
		textureWidth = width;
		return *this;
	}

	TextureLoader& TextureLoader::setHeight(uint32_t height)
	{
		textureHeight = height;
		return *this;
	}

	TextureLoader& TextureLoader::loadFromFile(const char* path)
	{
		if(path == nullptr)
			return *this;

		lodepng::State state;

		std::uint8_t*	data;
		std::size_t		size;

		// TODO : use custom file size check function
		// TODO : instead of using lodepngs load_file use their load buffer functions
		auto result = lodepng_load_file(&data, &size, path);

		if(result != 0 || size == 0)
		{
			Core::Logger::get()->warn (
				"TextureLoader : unable to load texture from file '{}', error code '{}', reason '{}'.",
				path, result, lodepng_error_text(result)
			);

			return *this;
		}

		uint32_t imageWidth, imageHeight;

		lodepng_inspect(&imageWidth, &imageHeight, &state, &data[0], size);
		LodePNGColorMode& color = state.info_png.color;

		Core::Logger::get()->info (
			"TextureLoader : BitDepth: '{}', width: '{}', height: '{}'.",
			lodepng_get_bpp(&color), imageWidth, imageHeight
		);

		switch(lodepng_get_bpp(&color))
		{
			case 8:
				format = gl::RGBA; // or GL_R, GL_R8, etc.
				pixelFormat = gl::R32F;
				break;
			case 16:
				format = gl::RGBA;
				pixelFormat = gl::RG32F;
				break;
			case 24:
				format = gl::RGBA;
				pixelFormat = gl::RGB32F;
				break;
			case 32:
				format = gl::RGBA;
				pixelFormat = gl::RGBA32F;
				break;
			default:
				Core::Logger::get()->error("TextureLoader : bitdepth '{}' is not supported!", state.info_png.color.bitdepth);
				return *this;
		}

		if(textureWidth == 0)
			textureWidth = imageWidth;

		if(textureHeight == 0)
			textureHeight = imageHeight;

		Memory::SafeFree(pixels);

		pixelsSize = lodepng_get_raw_size(textureWidth, textureHeight, &state.info_raw);
		lodepng_decode(&pixels, &textureWidth, &textureHeight, &state, data, size);

		Core::Logger::get()->info("TextureLoader : successfully loaded '{}'!", path);

		return *this;
	}

	TextureLoader::handle_t TextureLoader::build()
	{
		if(pixels == nullptr)
			return Texture::handle_t::invalid();

		handle_t textureHandle	= _manager.createTexture();
		Texture& texture		= _manager.getTexture(textureHandle);

		texture._width			= textureWidth;
		texture._height			= textureHeight;
		texture._format			= format;
		texture._pixelFormat	= pixelFormat;

		gl::GenTextures(1, &texture._handle);
		gl::BindTexture(gl::TEXTURE_2D, texture._handle);

		if(OpenGL::checkError())
		{
			Core::Logger::warn (
				"TextureLoader : Unable to bind texture '{}', due to openGl error!", texture._handle
			);

			gl::BindTexture(gl::TEXTURE_2D, 0);
			return Texture::handle_t::invalid();
		}

		setParameters(texture);

		// TODO : Texture MipMaps!
		/*if(MipMapLevel != 0)
		{
			glTexStorage2D(GL_TEXTURE_2D, MipMapLevel, GL_RGBA8, TextureWidth, TextureHeight);
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
				gl::UNSIGNED_BYTE, pixels
			);
		}

		if(OpenGL::checkError())
		{
			Core::Logger::error (
				"TextureLoader : Unable to create texture '{}' due to openGl error!", texture._handle
			);

			gl::BindTexture(gl::TEXTURE_2D, 0);
			return Texture::handle_t::invalid();
		}

		Core::Logger::info("TextureLoader : Texture '{}' created successfully!", texture._handle);

		gl::BindTexture(gl::TEXTURE_2D, 0);

		textureHeight	= 0;
		textureWidth	= 0;

		return textureHandle;
	}

	void TextureLoader::setParameters(const Texture &texture) const
	{
		gl::BindTexture(gl::TEXTURE_2D, texture._handle);

		for(auto& pair : parameters)
		{
			gl::TexParameteri(gl::TEXTURE_2D, pair.first, pair.second);
			if(OpenGL::checkError())
			{
				Core::Logger::warn (
					"TextureLoader : Unable to set parameter '{}' as '{}' due to openGL error!",
					pair.first, pair.second
				);
			}
		}

		gl::BindTexture(gl::TEXTURE_2D, 0);
		OpenGL::checkError();
	}

	TextureLoader& TextureLoader::setParameter(GLenum key, GLenum value)
	{
		parameters[key] = value;
		return *this;
	}
}
