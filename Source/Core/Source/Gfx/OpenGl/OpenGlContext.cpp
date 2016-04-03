//
// Created by MrJaqbq on 2016-03-23.
//

#include <Platform.h>
#include <Window.h>
#include "OpenGlContext.h"

namespace Gfx
{
	OpenGlContext::OpenGlContext()
	{

	}

	auto OpenGlContext::initialize() -> bool
	{
		// todo: pick from config
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		return true;
	}

	auto OpenGlContext::registerWindow(const Core::Window& window) -> bool
	{
		return true;
	}

	auto OpenGlContext::resizeWindow(const Core::Window& window) -> void
	{

	}

	auto OpenGlContext::destroyWindow(const Core::Window& window) -> bool
	{
		return true;
	}

	auto OpenGlContext::destroyWindows() -> void
	{

	}

	auto OpenGlContext::cleanUp() -> void
	{

	}

	void OpenGlContext::beginDraw(const Core::Window& window)
	{
		glfwMakeContextCurrent(window.hWindow);
		glClearColor(0.5f, 0.75f, 0.25f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGlContext::endDraw(const Core::Window& window)
	{
		glfwSwapBuffers(window.hWindow);
	}
}
