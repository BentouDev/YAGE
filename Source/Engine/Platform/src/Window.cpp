//
// Created by MrJaqbq on 2016-02-14.
//

#include "Platform/Logger.h"
#include "Platform/Window.h"
#include "Platform/Graphics/OpenGl/OpenGLBase.h"
#include "Platform/Platform.h"

namespace Core
{
	Window::Window(Memory::IMemoryBlock& memory, std::uintptr_t handle, const char* title, unsigned width, unsigned height)
		: _memory(&memory), Title{ title }, Width{ width }, Height{ height },
		DefaultViewport{ nullptr }, hWindow{ nullptr }, IsCloseRequested(false)
	{
		YAGE_ASSERT(false, "Not implemented!");
	}

    Window::Window(Memory::IMemoryBlock& memory, const char* title, unsigned width, unsigned height)
        : _memory(&memory), Title {title}, Width {width}, Height {height},
          DefaultViewport {nullptr}, hWindow {nullptr}, IsCloseRequested(false)
    {
        Create();
    }

    Window::~Window()
    {
        Destroy();
        Memory::Delete(*_memory, DefaultViewport);
    }

    auto Window::Create() -> void
    {
        if (IsAlive())
            return;

        hWindow = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);

        if (hWindow == nullptr)
        {
            Core::Logger::error("GLFW : Unable to create window!");
            return;
        }

        DefaultViewport = YAGE_CREATE_NEW((*_memory), Gfx::Viewport)(
            Gfx::Rectangle<int32_t>(0, 0, Width, Height),
            *this
        );
    }

    auto Window::Show() const noexcept -> void
    {
        if (!IsAlive())
            return;

        glfwShowWindow(hWindow);
    }

    void Window::OnResize(std::int32_t width, std::int32_t height)
    {
        if (width == Width && height == Height)
            return;

        Width  = width;
        Height = height;

        DefaultViewport->setRect(Gfx::Rectangle<int32_t>(0, 0, Width, Height));

        // OpenGL::resizeWindow(*this);
    }

    auto Window::Resize(std::int32_t width, std::int32_t height) -> void
    {
        glfwSetWindowSize(hWindow, width, height);
    }

    auto Window::Destroy() -> void
    {
        if (!IsAlive())
            return;

#ifdef _MSC_VER
        glfwDestroyWindow(hWindow);
#endif
        hWindow = nullptr;
    }

    auto Window::GetDefaultViewport() const noexcept -> Gfx::Viewport&
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
