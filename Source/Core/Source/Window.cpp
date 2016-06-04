//
// Created by MrJaqbq on 2016-02-14.
//

#include "Window.h"
#include "Platform.h"
#include "Gfx/Rectangle.h"
#include "Config.h"

namespace Core
{
	Window::Window(Context ctx) :
		hWindow {nullptr},
		Height {ctx.Config->WindowHeight},
		Width {ctx.Config->WindowWidth},
		Title {ctx.Config->WindowTitle},
		_viewports(8)
	{
		Create();
	}

	Window::~Window()
	{
		Destroy();
	//	Viewports.clear();
	//	_viewports.clear();
	}

	auto Window::Create() -> void
	{
		if(IsAlive())
			return;

		hWindow = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
	//	_viewports.create(Gfx::Rectangle<int32_t>());
	}

	auto Window::Show() const noexcept -> void
	{
		if(!IsAlive())
			return;

		glfwShowWindow(hWindow);
	}

	auto Window::Destroy() -> void
	{
		if(!IsAlive())
			return;

		glfwDestroyWindow(hWindow);
		hWindow = nullptr;
	}

	auto Window::GetDefaultViewport() -> Gfx::Viewport&
	{
		// todo: assert
		//assert(Viewports.size() > 0);
		return *Viewports[0];
	}

	auto Window::IsAlive() const noexcept -> bool
	{
		return hWindow != nullptr;
	}

	auto Window::ShouldClose() const noexcept -> bool
	{
		return !IsAlive() || glfwWindowShouldClose(hWindow);
	}

	auto Window::CreateViewport(const Gfx::Rectangle<int32_t>& rect) noexcept -> void
	{
	//	Viewports.push_back(new Gfx::Viewport(rect));
	//	_viewports.create(rect);
	}
}
