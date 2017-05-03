//
// Created by bentoo on 18.04.17.
//

#ifndef YAGE_TEXT_H
#define YAGE_TEXT_H

#include <Utils/String.h>
#include <Utils/Handle.h>
#include <Utils/MemoryBlock.h>

namespace Core
{
	class Material;
}

namespace Resources
{
	class Font;
}

namespace Gfx
{
	class Camera;
	class Renderer;
	class SpriteBatch;

	class TextBuilder
	{
		Memory::IMemoryBlock&			_memory;
		Utils::String					text;
		Resources::Font*				font;
		Utils::Handle<Core::Material>	material;

		void drawTextInternal(Utils::Slice<char> line, Gfx::SpriteBatch& batch);

	public:
		explicit TextBuilder(Memory::IMemoryBlock& memory);
		virtual ~TextBuilder();

		TextBuilder& appendText(const char* string);
		TextBuilder& withFont(Resources::Font& font);
		TextBuilder& withMaterial(Utils::Handle<Core::Material> material);
		void 		 drawAsSpriteBatch(Renderer& renderer, Gfx::Camera& camera);
	};
}

#endif //YAGE_TEXT_H
