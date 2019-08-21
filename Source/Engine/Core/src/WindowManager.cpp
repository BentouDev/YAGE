//
// Created by bentoo on 18.11.16.
//

#include <Platform/Logger.h>
#include <Platform/Platform.h>
#include <Platform/EventQueue.h>
#include "Core/WindowManager.h"

namespace Core
{
    WindowManager::WindowManager(Core::Engine &engine, Memory::IMemoryBlock &memory)
        : IManager(engine, memory), _windowContainer(memory, 1)
    {
        Core::Logger::info("Created window manager with capacity '{}'", _windowContainer.capacity());
    }

    WindowManager::~WindowManager()
    {

    }

    WindowManager::handle_t WindowManager::createNew(const char* name, unsigned width, unsigned height)
    {
        handle_t handle = _windowContainer.emplace(_memory, name, width, height);

        _windowIdMapper[reinterpret_cast<std::uintptr_t>(get(handle).hWindow)] = handle;

        return handle;
    }

    Window* WindowManager::tryGet(handle_t handle)
    {
        Window* result = nullptr;
        if (_windowContainer.contains(handle))
        {
            result = &_windowContainer.get(handle);
        }

        return result;
    }

    void WindowManager::closeAllWindows()
    {
        for (auto& window : _windowContainer)
        {
            window.Close();
        }
    }

    Window* WindowManager::getWindowById(std::uintptr_t id)
    {
        auto itr = _windowIdMapper.find(id);
        if (itr == _windowIdMapper.end())
            return nullptr;

        auto  handle = itr->second;
        auto* window = tryGet(handle);

        return window;
    }

    void WindowManager::handleWindowEvent(Window* window, const Event& event)
    {
        if (window == nullptr)
            return;

        switch (event.windowData.type)
        {
            case WindowEventType::FOCUS:
                break;
            case WindowEventType::LOST_FOCUS:
                break;
            case WindowEventType::MAXIMIZE:
                break;
            case WindowEventType::MINIMIZE:
                break;
            case WindowEventType::ICONIFY:
                break;
            case WindowEventType::MOVE:
                break;
            case WindowEventType::RESIZE:
                window->OnResize(event.windowData.coord.x, event.windowData.coord.y);
                break;
            case WindowEventType::DROP:
                break;
            case WindowEventType::CLOSE:
                window->IsCloseRequested = true;
                break;
            default:
                Core::Logger::debug("Window : '{}' UNKNOWN EVENT", window->Handle.key.raw);
                break;
        }
    }

    void WindowManager::handleWindowEvent(const Event& event)
    {
        auto* window = getWindowById(reinterpret_cast<std::uintptr_t>(event.window));
        if (window != nullptr)
        {
            handleWindowEvent(window, event);
        }
    }

    bool WindowManager::allWindowsClosed() const
    {
        bool result = true;

        for (auto& window : _windowContainer)
        {
            if (window.IsAlive() && !window.ShouldClose())
            {
                result = false;
                break;
            }
        }

        return result;
    }
}
