//
// Created by MrJaqbq on 2016-02-14.
//

#include "Core/Logger.h"
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

		hWindow = SDL_CreateWindow (
			Title.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			Width,
			Height,
			SDL_WINDOW_OPENGL
		);

		if(hWindow == nullptr)
		{
			Core::Logger::error("SDL : Unable to create window!\n\t{}", SDL_GetError());
			return;
		}

		hContext = SDL_GL_CreateContext(hWindow);

		const Gfx::Rectangle<int32_t> rect(0, 0, Width, Height);
		DefaultViewport = YAGE_CREATE_NEW(_memory, Gfx::Viewport)(rect);
	}

	auto Window::Show() const noexcept -> void
	{
		if(!IsAlive())
			return;

		SDL_ShowWindow(hWindow);
	}

	auto Window::Destroy() -> void
	{
		if(!IsAlive())
			return;

		SDL_GL_DeleteContext(hContext);
		SDL_DestroyWindow   (hWindow);
		hContext = nullptr;
		hWindow  = nullptr;
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
		return !IsAlive() || IsCloseRequested;
	}
}
