//
// Created by MrJaqbq on 2016-02-14.
//

#include "Window.h"
#include "Gfx/Rectangle.h"

namespace Core
{
	Window::Window() : hWindow(nullptr)
	{
		Create();
	}

	Window::~Window()
	{
		Destroy();
	}

	auto Window::Create() -> void
	{
		if(IsAlive())
			return;

		hWindow = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
	}

	auto Window::Destroy() -> void
	{
		if(!IsAlive())
			return;

		glfwDestroyWindow(hWindow);
		hWindow = nullptr;
	}

	auto Window::IsAlive() const noexcept -> bool
	{
		return hWindow != nullptr;
	}

	auto Window::CreateViewport(const Gfx::Rectangle<int32_t>& rect) noexcept -> void
	{
		Viewports.push_back(new Gfx::Viewport(rect));
	}
}
