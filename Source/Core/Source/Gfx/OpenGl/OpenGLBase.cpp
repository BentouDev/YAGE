//
// Created by Bentoo on 2016-09-22.
//

#include "OpenGLBase.h"

#include <Window.h>
#include "../../Platform.h"

namespace OpenGL
{
    gl::exts::LoadTest didLoadFunctions;

	auto initialize() -> bool
	{
		if(!glfwInit())
			return false;

		// todo: pick from config
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		return true;
	}

	void beginDraw(const Core::Window& window)
	{
		glfwMakeContextCurrent(window.hWindow);
		gl::ClearColor(0.5f, 0.75f, 0.25f, 0.0f);
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
	}

	void endDraw(const Core::Window& window)
	{
		glfwSwapBuffers(window.hWindow);
	}

    bool registerWindow(const Core::Window& window)
    {
		if(!didLoadFunctions)
        	didLoadFunctions = gl::sys::LoadFunctions();

        return didLoadFunctions;
    }

	void resizeWindow(const Core::Window& window)
	{

	}
}