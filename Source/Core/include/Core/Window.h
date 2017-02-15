//
// Created by MrJaqbq on 2016-02-14.
//

#ifndef YAGE_WINDOW_H
#define YAGE_WINDOW_H

#include <string>
#include <vector>
#include <Utils/List.h>

#include "Core/Gfx/Viewport.h"
#include "Core/Gfx/RenderTarget.h"

struct GLFWwindow;

namespace Core
{
	class Window
	{
		friend class WindowManager;

		void OnResize(std::int32_t width, std::int32_t);

	public:
		using handle_t = Utils::Handle<Window>;
		using trait_t = Utils::DefaultTrait<Window>;

		explicit Window(Memory::IMemoryBlock& memory, const char* title, unsigned width, unsigned height);
		virtual ~Window();

		Window(Window&& other)
			: _memory(other._memory),
			  Title(std::move(other.Title)),
			  Width(other.Width), Height(other.Height),
			  DefaultViewport(other.DefaultViewport),
			  hWindow(other.hWindow),
			  Handle(other.Handle)
		{
			other.DefaultViewport = nullptr;
			other.hWindow = nullptr;
		}

		Window(const Window&) = delete;
		Window& operator=(Window&&) = delete;
		Window& operator=(const Window&) = delete;

		auto Create() -> void;
		auto Destroy() -> void;
		auto IsAlive() const noexcept -> bool;
		auto ShouldClose() const noexcept -> bool;
		auto Show() const noexcept -> void;
		auto GetDefaultViewport() const noexcept -> Gfx::Viewport&;
		auto Resize(std::int32_t width, std::int32_t height) -> void;

		Memory::IMemoryBlock&	_memory;

		bool					IsCloseRequested;

		std::string				Title;
		unsigned				Width;
		unsigned				Height;

		Gfx::Viewport*			DefaultViewport;

		GLFWwindow*				hWindow;
		handle_t				Handle;

		auto operator==(const Window& other) -> bool
		{
			return this->hWindow == other.hWindow;
		}
	};
}

#endif //YAGE_WINDOW_H
