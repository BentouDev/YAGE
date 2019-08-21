//
// Created by MrJaqbq on 2016-02-14.
//

#ifndef YAGE_VIEWPORT_H
#define YAGE_VIEWPORT_H

#include <RTTI/Reflection.h>
#include "RenderTarget.h"
#include "Rectangle.h"
#include "BaseObject.h"

namespace Core
{
	class Window;
}

namespace Gfx
{
    // ToDo: this class is dangerous -> we should have only one base here
    // if needed split this into association
	class Viewport : public yage::SafeObject, public RenderTarget
	{
		Rectangle<int32_t>	_pixelRect;
		Rectangle<float>	_unitRect;
		const Core::Window*		_window;

		Rectangle<float> calcUnitRect();

	public:
		using handle_t	= Utils::Handle<Viewport>;

		handle_t Handle;

		explicit Viewport(const Rectangle<int32_t>& rect, const Core::Window& window)
			: _pixelRect(rect), _unitRect(calcUnitRect()), _window(&window)
		{ }

		void	Clear(const Utils::Color& color) override;
		void	Bind() override;
		void	Submit() override;

		float	getAspect() override;
		auto	getPixelRect() -> const Rectangle<int32_t>& override;
		auto 	getUnitRect() -> const Rectangle<float>& override;
		void	setRect(const Rectangle<int32_t>& rect);
	};
}

YAGE_DECLARE_CLASS_RTTI(Gfx::Viewport);

#endif //YAGE_VIEWPORT_H