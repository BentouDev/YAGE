//
// Created by Bentoo on 2017-04-02.
//

#include <cmath>
#include <Utils/Math.h>
#include "Core/Gfx/SpriteBatchManager.h"
#include "Core/Gfx/Sprite.h"
#include "Core/Gfx/SpriteBatch.h"
#include "Core/Gfx/Camera.h"
#include "Core/Resources/Material/Material.h"

namespace Gfx
{
    SpriteBatch::SpriteBatch(SpriteBuffer &buffer)
        : _buffer(buffer),
          _bufferOffset(buffer.currentSize),
          _bufferSize(0),
          _materialPtr(nullptr), _cameraPtr(nullptr),
          defaultTexRect(0, 0, 1, 1),
          defaultPivot(0.5f, 0.5f),
          defaultColor(255,255, 255, 255),
          defaultRotation(0), defaultScale(1),
          defaultZOrder(0),
          defaultLayer(0),
          batchZOrder(0),
          blendEnabled(true),
          blendSfactor(gl::SRC_ALPHA),
          blendDfactor(gl::ONE_MINUS_SRC_ALPHA),
          depthEnabled(true),
          depthFunc(gl::LEQUAL)
    { }

    SpriteBatch& SpriteBatch::clear()
    {
        _bufferOffset = 0;
        _bufferSize = 0;

        return *this;
    }

    SpriteBatch& SpriteBatch::ensureCapacity(std::int32_t minimalCapacity)
    {
        YAGE_ASSERT(_buffer.maximumSize - _buffer.currentSize >= minimalCapacity,
                    "TODO : Batch resize, '{}' was smaller than expected capacity '{}'.",
                    _buffer.maximumSize - _buffer.currentSize, minimalCapacity);

        _bufferSize += minimalCapacity;
        _bufferOffset = _buffer.offset;

        return *this;
    }

    SpriteBatch& SpriteBatch::drawSprite(Rectangle<float> rect, Utils::Color color)
    {
        return drawSprite(rect, defaultTexRect, defaultScale, defaultRotation, color);
    }

    SpriteBatch& SpriteBatch::drawSprite(Rectangle<float> rect, Rectangle<float> texRect,
                                         float scale, float rotation, Utils::Color color)
    {
        return drawSprite(rect, defaultPivot, texRect, defaultLayer, scale, rotation, defaultZOrder, color);
    }

    SpriteBatch& SpriteBatch::drawSprite(Rectangle<float> rect, glm::vec2 pivot, Rectangle<float> texRect,
                                         std::uint8_t texLayer, float scale, float rotation, float zOrder, Utils::Color color)
    {
        ensureCapacity(6);
        SpriteVertex data[6];
        Sprite::fillVertexData(data, rect, texRect, texLayer, zOrder, color);
        scaleAndRotateSpriteVertex(data, rect.localToWorld(pivot), scale, rotation);
        _buffer.copyData(data);

        return *this;
    }

    SpriteBatch& SpriteBatch::drawSprite(const Sprite& sprite)
    {
        ensureCapacity(6);
        SpriteVertex data[6];
        sprite.fillVertexData(data);
        scaleAndRotateSpriteVertex(data, sprite.getRect().localToWorld(sprite.getPivot()),
                                   sprite.getScale(), sprite.getRotation());
        _buffer.copyData(data);

        return *this;
    }

    void SpriteBatch::scaleAndRotateSpriteVertex(SpriteVertex (&data)[6], glm::vec2 pivot, float scale, float rotation)
    {
        if(abs(rotation) > Math::Epsilon)
        {
            const float sine	= sinf(Math::AngleToRadian(rotation));
            const float cosine	= cosf(Math::AngleToRadian(rotation));

            for(int i = 0; i < 6; i++)
            {
                data[i].position.x	-= pivot.x;
                data[i].position.y	-= pivot.y;

                const glm::vec3 oldPos = data[i].position;
                data[i].position.x	 = cosine * oldPos.x - sine * oldPos.y;
                data[i].position.y	 = sine * oldPos.x + cosine * oldPos.y;

                data[i].position.x	*= scale;
                data[i].position.y	*= scale;

                data[i].position.x	+= pivot.x;// * scale;
                data[i].position.y	+= pivot.y;// * scale;
            }
        }
        else
        {
            for(int i = 0; i < 6; i++)
            {
                data[i].position.x  -= pivot.x;
                data[i].position.y  -= pivot.y;
                data[i].position.x	*= scale;
                data[i].position.y	*= scale;
                data[i].position.x  += pivot.x;
                data[i].position.y  += pivot.y;
            }
        }
    }
}
