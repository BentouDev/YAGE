//
// Created by Bentoo on 2017-04-02.
//

#ifndef YAGE_SPRITEBATCH_H
#define YAGE_SPRITEBATCH_H

#include <cstdint>
#include "Core/Gfx/Rectangle.h"

#include <Utils/MemoryBlock.h>
#include <Utils/Color.hpp>

namespace Core
{
	class Material;
}

namespace Gfx
{
	class Sprite;
	class Camera;
	struct SpriteVertex;
	class SpriteBatch;
	class SpriteBatchManager;

	class SpriteBatch
	{
		friend class SpriteBatchManager;

	private:
		Gfx::SpriteBuffer&		_buffer;

		std::uint32_t			_bufferOffset;
		std::uint32_t			_bufferSize;

		Core::Material*	 		_materialPtr;
		Gfx::Camera*			_cameraPtr;

		Rectangle<float>	defaultTexRect;
		Utils::Color		defaultColor;
		glm::vec2			defaultPivot;
		float				defaultRotation;
		float				defaultScale;


		void scaleAndRotateSpriteVertex(SpriteVertex (&data)[6], glm::vec2 pivot, float scale, float rotation);

	public:
		GLenum				blendSfactor;
		GLenum				blendDfactor;

		explicit SpriteBatch(SpriteBuffer& buffer);

		SpriteBatch(const SpriteBatch& other)
			: _buffer(other._buffer),
			  _bufferOffset(other._bufferOffset),
			  _bufferSize(other._bufferSize),
			  _materialPtr(other._materialPtr),
			  _cameraPtr(other._cameraPtr),
			  defaultTexRect(other.defaultTexRect),
			  defaultColor(other.defaultColor),
			  defaultRotation(other.defaultRotation),
			  defaultScale(other.defaultScale)
		{ }

		SpriteBatch(SpriteBatch&& other)
			: _buffer(other._buffer),
			  _bufferOffset(other._bufferOffset),
			  _bufferSize(other._bufferSize),
			  _materialPtr(other._materialPtr),
			  _cameraPtr(other._cameraPtr),
			  defaultTexRect(other.defaultTexRect),
			  defaultColor(other.defaultColor),
			  defaultRotation(other.defaultRotation),
			  defaultScale(other.defaultScale)
		{
			other._materialPtr = nullptr;
			other._cameraPtr = nullptr;
		}

		SpriteBatch& clear();
		SpriteBatch& ensureCapacity(std::int32_t minimalCapacity);
		SpriteBatch& drawSprite(const Sprite& sprite);
		SpriteBatch& drawSprite(Rectangle<float> rect, Utils::Color color);
		SpriteBatch& drawSprite(Rectangle<float> rect, Rectangle<float> texRect,
								float scale, float rotation, Utils::Color color);
		SpriteBatch& drawSprite(Rectangle<float> rect, glm::vec2 pivot, Rectangle<float> texRect,
								float scale, float rotation, Utils::Color color);

		inline Gfx::Camera* getCamera() const { return _cameraPtr; }
		inline Core::Material* getMaterial() const { return _materialPtr; }
		inline Gfx::SpriteBuffer& getBuffer() const { return _buffer; }
		inline std::uint32_t getOffset() const { return _bufferOffset; }
		inline std::uint32_t getSize() const { return _bufferSize; }
	};
}

#endif //YAGE_SPRITEBATCH_H
