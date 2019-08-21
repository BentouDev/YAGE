//
// Created by MrJaqbq on 2016-02-14.
//

#ifndef YAGE_WINDOW_H
#define YAGE_WINDOW_H

#include <vector>
#include <Utils/List.h>
#include <Utils/String.h>
#include <RTTI/Reflection.h>

#include "Platform.h"
#include "BaseObject.h"

namespace Core
{
    class Window : public yage::SafeObject
    {
        friend class WindowManager;

        void OnResize(std::int32_t width, std::int32_t);

    public:
        using handle_t = Utils::Handle<Window>;

        Window() : _memory{ nullptr }, hWindow{ nullptr }
        { }

        explicit Window(Memory::IMemoryBlock& memory, const char* title, unsigned width, unsigned height);
        virtual ~Window();

        Window(Window&& other)
            : _memory(other._memory)
            , Title(std::move(other.Title))
            , Width(other.Width), Height(other.Height)
            , hWindow(other.hWindow)
            , Handle(other.Handle)
        {
            other.hWindow = nullptr;
        }

        Window(const Window&) = delete;
        Window& operator=(Window&&) = delete;
        Window& operator=(const Window&) = delete;

        void Show() const noexcept;
        void Close();
        bool IsAlive() const noexcept;
        bool ShouldClose() const noexcept;
        void Resize(std::int32_t width, std::int32_t height);
        auto GetNative() const->yage::platform::WindowHandle;

        Utils::String Title;
        unsigned      Width;
        unsigned      Height;

        handle_t Handle;

        auto operator==(const Window& other) -> bool
        {
            return this->hWindow == other.hWindow;
        }

    private:
        void Destroy();
        void Create();

        yage::platform::WindowHandle hWindow;

        Memory::IMemoryBlock* _memory;

        bool IsCloseRequested;
    };
}

YAGE_DECLARE_CLASS_RTTI(Core::Window);

#endif //YAGE_WINDOW_H