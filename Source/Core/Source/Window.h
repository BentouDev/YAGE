//
// Created by MrJaqbq on 2016-02-14.
//

#ifndef VOLKHVY_WINDOW_H
#define VOLKHVY_WINDOW_H

#include <string>
#include <vector>
#include "Platform.h"
#include "Gfx/Viewport.h"

namespace Core
{
	class Window
	{
	public:
		Window();
		virtual ~Window();

		auto Create() -> void;
		auto Destroy() -> void;
		auto IsAlive() const noexcept -> bool;
		auto CreateViewport(const Gfx::Rectangle<int32_t>& rect) noexcept -> void;

		std::string Title;
		unsigned	Width;
		unsigned	Height;

		// Replace with handle/pointer, store viewport elswhere (renderer? context?)
		std::vector<Gfx::Viewport*> Viewports;

		//Todo: wrap handle into smart pointer
		GLFWwindow* hWindow;
	};
}

#endif //VOLKHVY_WINDOW_H
