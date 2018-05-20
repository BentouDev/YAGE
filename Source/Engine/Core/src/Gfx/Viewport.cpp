//
// Created by MrJaqbq on 2016-02-14.
//

#include "Core/Gfx/Viewport.h"
#include "Core/Gfx/OpenGl/OpenGLBase.h"
#include "Core/Window.h"
#include "Core/Platform.h"

namespace Gfx
{
	void Viewport::Clear(const Utils::Color& color)
	{
		glfwMakeContextCurrent(_window.hWindow);
		gl::ClearColor(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f);
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
	}

	void Viewport::Submit()
	{
		glfwSwapBuffers(_window.hWindow);
	}

	void Viewport::Bind()
	{
		glfwMakeContextCurrent(_window.hWindow);
		gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
		gl::Viewport(_pixelRect.getLeft(), _pixelRect.getBottom(),
					 _pixelRect.getWidth(), _pixelRect.getHeight());
	}

	float Viewport::getAspect()
	{
		return (float)_pixelRect.getWidth() / (float)_pixelRect.getHeight();
	}

	Rectangle<float> Viewport::calcUnitRect()
	{
		if (_pixelRect.getWidth() > _pixelRect.getHeight())
		{
			return Rectangle<float> (
				-getAspect(), -1,
				2 * getAspect(), 2
			);
		}
		else
		{
			return Rectangle<float> (
				-1, -getAspect(),
				2, 2 * getAspect()
			);
		}
	}

	const Rectangle<int32_t>& Viewport::getPixelRect()
	{
		return _pixelRect;
	}

	const Rectangle<float>& Viewport::getUnitRect()
	{
		return _unitRect;
	}

	void Viewport::setRect(const Rectangle<int32_t>& rect)
	{
		_pixelRect = rect;
		_unitRect  = calcUnitRect();
	}
}
