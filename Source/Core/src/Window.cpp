//
// Created by MrJaqbq on 2016-02-14.
//

#include "Core/Window.h"
#include "Core/Platform.h"
#include "Core/Gfx/Viewport.h"

namespace Core
{
	Window::Window(Memory::IMemoryBlock& memory, const char* title, unsigned width, unsigned height)
		: _memory(memory), Title {title}, Width {width}, Height {height},
		  DefaultViewport{nullptr}, hWindow {nullptr}
	{
		Create();
	}

	Window::~Window()
	{
		Destroy();
		Memory::Delete(_memory, DefaultViewport);
	}

	auto Window::Create() -> void
	{
		if(IsAlive())
			return;

		hWindow = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);
		const Gfx::Rectangle<int32_t> rect(0, 0, Width, Height);
		DefaultViewport = YAGE_CREATE_NEW(_memory, Gfx::Viewport)(rect);
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
		return *DefaultViewport;
	}

	auto Window::IsAlive() const noexcept -> bool
	{
		return hWindow != nullptr;
	}

	auto Window::ShouldClose() const noexcept -> bool
	{
		return !IsAlive() || glfwWindowShouldClose(hWindow);
	}
}
