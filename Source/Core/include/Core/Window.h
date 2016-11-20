//
// Created by MrJaqbq on 2016-02-14.
//

#ifndef YAGE_WINDOW_H
#define YAGE_WINDOW_H

#include <string>
#include <vector>
#include <Utils/Container.h>
#include "Core/Gfx/Viewport.h"
#include "Core/Gfx/RenderTarget.h"
#include "Context.h"

struct GLFWwindow;

namespace Core
{
	class Window
	{
	public:
		using handle_t = Utils::Handle<Window>;
		using trait_t = Utils::DefaultTrait<Window>;

		explicit Window(const char* title, unsigned width, unsigned height);
		virtual ~Window();

		Window(Window&& other)
			: Title(std::move(other.Title)),
			  Width(other.Width), Height(other.Height),
			  Viewports(std::move(other.Viewports)),
			  hWindow(other.hWindow)
		{
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
		auto CreateViewport(const Gfx::Rectangle<int32_t>& rect) noexcept -> void;
		auto GetDefaultViewport() -> Gfx::Viewport&;

		std::string Title;
		unsigned	Width;
		unsigned	Height;

		// Replace with handle/pointer, store viewport elswhere (renderer? context?)
		std::vector<Gfx::Viewport*> Viewports;
	//	Utils::Container<typename Gfx::Viewport::Trait> _viewports;

		//Todo: wrap handle into smart pointer
		GLFWwindow* hWindow;
		handle_t Handle;

		auto operator==(const Window& other) -> bool
		{
			return this->hWindow == other.hWindow;
		}
	};
}

#endif //YAGE_WINDOW_H
