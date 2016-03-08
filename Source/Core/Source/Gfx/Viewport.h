//
// Created by MrJaqbq on 2016-02-14.
//

#ifndef VOLKHVY_VIEWPORT_H
#define VOLKHVY_VIEWPORT_H

#include "RenderTarget.h"
#include "Rectangle.h"

namespace Gfx
{
	class Viewport : public RenderTarget
	{
	public:
		explicit Viewport(const Rectangle<int32_t>& rect)
		{

		}
	};
}

#endif //VOLKHVY_VIEWPORT_H
