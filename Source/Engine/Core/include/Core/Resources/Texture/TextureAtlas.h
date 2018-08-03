//
// Created by bentoo on 11.05.17.
//

#ifndef YAGE_TEXTUREATLAS_H
#define YAGE_TEXTUREATLAS_H

#include <Utils/Handle.h>
#include <Utils/List.h>

namespace Resources
{
	class Texture;
	class TextureRegion;
	struct TextureRegionData;

	class TextureAtlas
	{
		Utils::List<TextureRegion>	regions;
		handle_t					textureHandle;

	public:
		using handle_t = Utils::Handle<Texture>;

		explicit TextureAtlas(Memory::IMemoryBlock& memory);
		virtual ~TextureAtlas();
	};
}

#endif //YAGE_TEXTUREATLAS_H
