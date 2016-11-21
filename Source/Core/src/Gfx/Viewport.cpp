//
// Created by MrJaqbq on 2016-02-14.
//

#include "Core/Gfx/Viewport.h"

namespace Gfx
{
	void Viewport::Clear(const Utils::Color&)
	{

	}

	void Viewport::Submit()
	{

	}

	void Viewport::Bind()
	{

	}

	float Viewport::getAspect()
	{
		return (float)_rect.getWidth() / (float)_rect.getHeight();
	}
}
