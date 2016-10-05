//
// Created by MrJaqbq on 2016-02-14.
//

#ifndef VOLKHVY_VIEWPORT_H
#define VOLKHVY_VIEWPORT_H

#include <Utils/DefaultTrait.h>
#include "RenderTarget.h"
#include "Rectangle.h"

namespace Gfx
{
	class Viewport : public RenderTarget
	{
		Rectangle<int32_t> _rect;

	public:
		using Trait = Utils::DefaultTrait<Viewport>;

		Utils::Handle<Viewport> Handle;

		explicit Viewport(const Rectangle<int32_t>& rect) : _rect(rect)
		{ }

		// required for trait
		void cleanUp();
		void swap(Viewport& other);

		void Clear(const Utils::Color& color) override;
		void Submit() override;
	};
}

#endif //VOLKHVY_VIEWPORT_H
