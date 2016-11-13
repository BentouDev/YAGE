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

	void Viewport::cleanUp()
	{
		
	}

	void Viewport::swap(Viewport &other)
	{
		Handle.swap(other.Handle);
	}
}
