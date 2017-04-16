//
// Created by MrJaqbq on 2016-02-14.
//

#ifndef YAGE_VIEWPORT_H
#define YAGE_VIEWPORT_H

#include <Utils/DefaultTrait.h>
#include "RenderTarget.h"
#include "Rectangle.h"

namespace Core
{
	class Window;
}

namespace Gfx
{
	class Viewport : public RenderTarget
	{
		Rectangle<int32_t>	_rect;
		Core::Window&		_window;

	public:
		using trait_t	= Utils::DefaultTrait<Viewport>;
		using handle_t	= Utils::Handle<Viewport>;

		handle_t Handle;

		explicit Viewport(const Rectangle<int32_t>& rect, Core::Window& window)
			: _rect(rect), _window(window)
		{ }

		void	Clear(const Utils::Color& color) override;
		void	Bind() override;
		void	Submit() override;

		float	getAspect() override;
		auto	getRect() -> const Rectangle<int32_t>& override;
		void	setRect(const Rectangle<int32_t>& rect);
	};
}

#endif //YAGE_VIEWPORT_H
