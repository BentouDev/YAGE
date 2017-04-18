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
		gl::Viewport(_rect.getLeft(), _rect.getBottom(), _rect.getWidth(), _rect.getHeight());
	}

	float Viewport::getAspect()
	{
		return (float)_rect.getWidth() / (float)_rect.getHeight();
	}

	const Rectangle<int32_t>& Viewport::getRect()
	{
		return _rect;
	}

	void Viewport::setRect(const Rectangle<int32_t>& rect)
	{
		_rect = rect;
	}
}
