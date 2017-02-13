//
// Created by bentoo on 18.11.16.
//

#include "Core/WindowManager.h"
#include "Core/Logger.h"

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
		handle_t handle = _windowContainer.create(_memory, name, width, height);
		return handle;
	}

	Window* WindowManager::tryGet(handle_t handle)
	{
		if(_windowContainer.contains(handle))
		{
			return &_windowContainer.get(handle);
		}

		return nullptr;
	}

	void WindowManager::closeAllWindows()
	{
		for(auto& window : _windowContainer)
		{
			window.Destroy();
		}
	}
}
