//
// Created by mrjaqbq on 07.03.16.
//

#ifndef YAGE_RENDERTARGET_H
#define YAGE_RENDERTARGET_H

#include <Utils/Color.hpp>
#include "Rectangle.h"

namespace Gfx
{
	class RenderTarget
	{
	public:
		virtual ~RenderTarget();
		virtual void Bind() = 0;
		virtual void Clear(const Utils::Color& color) = 0;
		virtual void Submit() = 0;

		static void Reset();

		virtual float getAspect() = 0;
		virtual const Rectangle<int32_t>& getPixelRect() = 0;
		virtual const Rectangle<float>& getUnitRect() = 0;
	};
}

#endif //YAGE_RENDERTARGET_H
