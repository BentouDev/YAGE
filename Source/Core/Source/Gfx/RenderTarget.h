//
// Created by mrjaqbq on 07.03.16.
//

#ifndef YAGE_RENDERTARGET_H
#define YAGE_RENDERTARGET_H

#include <Utils/Color.hpp>

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
	};
}

#endif //YAGE_RENDERTARGET_H
