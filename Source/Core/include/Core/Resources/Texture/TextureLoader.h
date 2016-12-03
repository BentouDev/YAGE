//
// Created by bentoo on 03.12.16.
//

#ifndef GAME_TEXTURELOADER_H
#define GAME_TEXTURELOADER_H

#include <map>
#include <Utils/Handle.h>
#include "Core/Gfx/OpenGl/OpenGLBase.h"

namespace Resources
{
	class Texture;
	class TextureManager;

	// Debug implementation!
	class TextureLoader
	{
		TextureManager& _manager;

		std::size_t	pixelsSize;
		uint8_t*	pixels;

		GLenum		format;
		GLenum		pixelFormat;
		uint32_t	textureWidth;
		uint32_t	textureHeight;

		std::map<GLenum, GLenum> parameters;

		void setParameters(const Texture& texture) const;

	public:
		using handle_t = Utils::Handle<Texture>;

		explicit TextureLoader(TextureManager& manager);
		virtual ~TextureLoader();

		TextureLoader& setParameter(GLenum key, GLenum value);
		TextureLoader& loadFromFile(const char* path);
		TextureLoader& setWidth(uint32_t width);
		TextureLoader& setHeight(uint32_t height);
		handle_t build();
	};
}

#endif //GAME_TEXTURELOADER_H
