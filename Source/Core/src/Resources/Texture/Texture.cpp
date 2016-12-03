//
// Created by bentoo on 01.12.16.
//

#include <Utils/Using.h>
#include "Core/Resources/Texture/Texture.h"

namespace Resources
{
	Texture::Texture()
		: _pixels(nullptr), _width(0), _height(0),
		  _handle(0), _format(0), _pixelFormat(0)
	{

	}

	Texture::Texture(Texture&& other)
		: _pixels(other._pixels), _width(other._width), _height(other._height),
		  _handle(other._handle), _format(other._format), _pixelFormat(other._pixelFormat)
	{
		other._pixels = nullptr;
		other._handle = 0;
	}

	Texture::~Texture()
	{
		Memory::SafeFree(_pixels);

		if(_handle > 0)
		{
			gl::DeleteTextures(1, &_handle);
		}
	}
}
