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
		auto Clear(const Utils::Color& color) noexcept -> void;
	};
}

#endif //VOLKHVY_RENDERTARGET_H
