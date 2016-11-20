//
// Created by MrJaqbq on 2016-02-14.
//

#ifndef YAGE_VIEWPORT_H
#define YAGE_VIEWPORT_H

#include <Utils/DefaultTrait.h>
#include "RenderTarget.h"
#include "Rectangle.h"

namespace Gfx
{
	class Viewport : public RenderTarget
	{
		Rectangle<int32_t> _rect;

	public:
		using trait_t = Utils::DefaultTrait<Viewport>;
		using handle_t = Utils::Handle<Viewport>;

		handle_t Handle;

		explicit Viewport(const Rectangle<int32_t>& rect) : _rect(rect)
		{ }

		void Clear(const Utils::Color& color) override;
		void Submit() override;
	};
}

#endif //YAGE_VIEWPORT_H
