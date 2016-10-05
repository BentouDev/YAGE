//
// Created by MrJaqbq on 2016-02-14.
//

#ifndef VOLKHVY_WINDOW_H
#define VOLKHVY_WINDOW_H

#include <string>
#include <vector>
#include <Utils/Container.h>
#include "Gfx/Viewport.h"
#include "Gfx/RenderTarget.h"
#include "Context.h"

struct GLFWwindow;

namespace Core
{
	class Window
	{
	public:
		Window(Context ctx);
		virtual ~Window();

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

		auto operator==(const Window& other) -> bool
		{
			return this->hWindow == other.hWindow;
		}
	};

	/*auto operator==(const Window& first, const Window& second) -> bool
	{
		return first.hWindow == second.hWindow;
	}*/
}

#endif //VOLKHVY_WINDOW_H
