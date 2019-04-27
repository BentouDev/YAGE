//
// Created by MrJaqbq on 2016-02-14.
//

#ifndef YAGE_WINDOW_H
#define YAGE_WINDOW_H

#include <vector>
#include <Utils/List.h>
#include <Utils/String.h>

#include "Platform.h"
#include "Graphics/Viewport.h"
#include "Graphics/RenderTarget.h"
#include "BaseObject.h"

namespace Core
{
	class Window : public yage::SafeObject
	{
		friend class WindowManager;

		void OnResize(std::int32_t width, std::int32_t);

	public:
		using handle_t = Utils::Handle<Window>;

		Window() : _memory { nullptr }, hWindow{ nullptr }, DefaultViewport { nullptr }
		{ }

		explicit Window(Memory::IMemoryBlock& memory, const char* title, unsigned width, unsigned height);
		explicit Window(Memory::IMemoryBlock& memory, std::uintptr_t handle, const char* title, unsigned width, unsigned height);
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

		void Create();
		void Destroy();
		void Show() const noexcept;
		void Close();
		bool IsAlive() const noexcept;
		bool ShouldClose() const noexcept;
		void Resize(std::int32_t width, std::int32_t height);
		auto GetDefaultViewport() const noexcept->Gfx::Viewport &;
		auto GetNative() const -> yage::platform::WindowHandle;

		Utils::String Title;
		unsigned      Width;
		unsigned      Height;

		handle_t Handle;

		auto operator==(const Window& other) -> bool
		{
			return this->hWindow == other.hWindow;
		}

	private:
		yage::platform::WindowHandle hWindow;

		Gfx::Viewport* DefaultViewport;

		Memory::IMemoryBlock* _memory;

		bool IsCloseRequested;
	};
}

#endif //YAGE_WINDOW_H
