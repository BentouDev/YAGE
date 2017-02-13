//
// Created by bentoo on 18.11.16.
//

#include "Core/WindowManager.h"
#include "Core/Logger.h"
#include "Core/Platform.h"

namespace Core
{
	WindowManager::WindowManager(Core::Engine &engine, Memory::IMemoryBlock &memory)
		: _engine(engine), _memory(memory), _windowContainer(memory, 1)
	{
		Core::Logger::get()->info("Created window manager with capacity '{}'", _windowContainer.capacity());
	}

	WindowManager::~WindowManager()
	{

	}

	WindowManager::handle_t WindowManager::createNew(const char *name, unsigned width, unsigned height)
	{
		handle_t	handle	= _windowContainer.create(_memory, name, width, height);
		auto		id		= SDL_GetWindowID(get(handle).hWindow);

		_windowIdMapper[id] = handle;

		return handle;
	}

	Window* WindowManager::tryGet(handle_t handle)
	{
		Window* result = nullptr;
		if(_windowContainer.contains(handle))
		{
			result = &_windowContainer.get(handle);
		}

		return result;
	}

	void WindowManager::closeAllWindows()
	{
		for(auto& window : _windowContainer)
		{
			window.Destroy();
		}
	}

	Window* WindowManager::getWindowById(std::uint32_t id)
	{
		auto itr = _windowIdMapper.find(id);
		if (itr == _windowIdMapper.end())
			return nullptr;

		auto  handle = itr->second;
		auto* window = tryGet(handle);

		return window;
	}

	void WindowManager::onResizeWindow(std::uint32_t id, std::int32_t width, std::int32_t height)
	{
		auto* window = getWindowById(id);
		if(window == nullptr)
			return;

		window->Resize(width, height);
	}

	void WindowManager::onCloseWindow(std::uint32_t id)
	{
		auto* window = getWindowById(id);
		if(window == nullptr)
			return;

		window->IsCloseRequested = true;
	}

	void WindowManager::handleWindowEvent(const SDL_Event& event)
	{
		const SDL_WindowEvent& window = event.window;
		switch (window.event)
		{
			case SDL_WINDOWEVENT_SHOWN:
				Core::Logger::debug("Window : '{}' shown.", window.windowID );
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				Core::Logger::debug("Window : '{}' hidden.", window.windowID );
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				Core::Logger::debug("Window : '{}' exposed.", window.windowID);
				break;
			case SDL_WINDOWEVENT_MOVED:
				Core::Logger::debug("Window : '{}' moved to '{}' x '{}'.",
								   window.windowID, window.data1, window.data2);
				break;
			case SDL_WINDOWEVENT_RESIZED:
				onResizeWindow(window.windowID, window.data1, window.data2);
				Core::Logger::info("Window : '{}' resized to '{}' x '{}'.",
								   window.windowID, window.data1, window.data2);
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				Core::Logger::debug("Window : '{}' minimized.", window.windowID);
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				Core::Logger::debug("Window : '{}' maximized.", window.windowID);
				break;
			case SDL_WINDOWEVENT_RESTORED:
				Core::Logger::debug("Window : '{}' restored.", window.windowID);
				break;
			case SDL_WINDOWEVENT_ENTER:
				Core::Logger::debug("Window : '{}' mouse enter.", window.windowID);
				break;
			case SDL_WINDOWEVENT_LEAVE:
				Core::Logger::debug("Window : '{}' mouse leave.", window.windowID);
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				Core::Logger::debug("Window : '{}' focus gained.", window.windowID);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				Core::Logger::debug("Window : '{}' focus lost.", window.windowID);
				break;
			case SDL_WINDOWEVENT_CLOSE:
				onCloseWindow(window.windowID);
				Core::Logger::debug("Window : '{}' closed.", window.windowID);
				break;
			default:
				Core::Logger::debug("Window : '{}' UNKNOWN EVENT", window.windowID);
				break;
		}
	}
}
