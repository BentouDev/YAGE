//
// Created by bentoo on 03.12.16.
//

#ifndef GAME_TEXTURELOADER_H
#define GAME_TEXTURELOADER_H

#include <map>
#include <Utils/Handle.h>
#include <Utils/List.h>
#include "Core/Gfx/OpenGl/OpenGLBase.h"

namespace Resources
{
	class Texture;
	class TextureManager;

	namespace MultiTextureType
	{
		enum Enum : std::int8_t
		{
			Texture2DArray,
			Texture3D,
			_COUNT
		};
	}

	class TextureLoader
	{
		TextureManager& _manager;

		struct TextureData
		{
			TextureData()
			: pixels(nullptr)
			, pixelsSize(0)
		  	, format(0)
		  	, pixelFormat(0)
		  	, textureWidth(0)
		  	, textureHeight(0)
			{ }

			uint8_t*	pixels;
			std::size_t	pixelsSize;

			GLenum		format;
			GLenum		pixelFormat;
			uint32_t	textureWidth;
			uint32_t	textureHeight;
		};

		Utils::List<TextureData> _data;
		std::map<GLenum, GLenum> parameters;

		void setParameters(GLenum slot, const Texture& texture) const;

		bool validateSameParameters();

	public:
		using handle_t = Utils::Handle<Texture>;

		explicit TextureLoader(TextureManager& manager, Memory::IMemoryBlock& memory);
		virtual ~TextureLoader();

		TextureLoader& setParameter(GLenum key, GLenum value);
		TextureLoader& loadFromFile(const char* path);

		handle_t buildAsMulti(MultiTextureType::Enum type);
		void buildAsList(Utils::List<handle_t>& handles);
		handle_t buildAsSingle();
	};
}

#endif //GAME_TEXTURELOADER_H
