//
// Created by mrjaqbq on 07.03.16.
//

#ifndef VOLKHVY_RENDERTARGET_H
#define VOLKHVY_RENDERTARGET_H

#include <Color.hpp>

namespace Gfx
{
	class RenderTarget
	{
	public:
		virtual ~RenderTarget();
		virtual void Clear(const Utils::Color& color) = 0;
		virtual void Submit() = 0;
	};
}

#endif //VOLKHVY_RENDERTARGET_H
