//
// Created by bentoo on 18.11.16.
//

#ifndef GAME_WINDOWMANAGER_H
#define GAME_WINDOWMANAGER_H

#include <Utils/MemoryBlock.h>
#include <Utils/Container.h>
#include "Core/Window.h"

namespace Core
{
	class Engine;

	class WindowManager
	{
	public:
		using handle_t = Utils::Handle<Core::Window>;

	protected:
		Core::Engine& _engine;
		Memory::IMemoryBlock& _memory;

		Utils::Container<Window::trait_t> _windowContainer;

	public:
		explicit WindowManager(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~WindowManager();

		handle_t createNew(const char* name, unsigned width, unsigned height);

		Window& get(handle_t handle)
		{ return _windowContainer.get(handle); }

		Window* tryGet(handle_t);
	};
}

#endif //GAME_WINDOWMANAGER_H
