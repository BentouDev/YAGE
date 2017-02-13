//
// Created by bentoo on 18.11.16.
//

#ifndef GAME_WINDOWMANAGER_H
#define GAME_WINDOWMANAGER_H

#include <map>
#include <Utils/MemoryBlock.h>
#include <Utils/Container.h>
#include "Core/Window.h"

union SDL_Event;

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

		std::map<std::uint32_t, Window::handle_t> _windowIdMapper;

		Window* getWindowById(std::uint32_t id);

		void onResizeWindow(std::uint32_t id, std::int32_t width, std::int32_t height);
		void onCloseWindow (std::uint32_t id);

	public:
		explicit WindowManager(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~WindowManager();

		handle_t createNew(const char* name, unsigned width, unsigned height);

		Window& get(handle_t handle)
		{ return _windowContainer.get(handle); }

		Window* tryGet(handle_t);

		void closeAllWindows();
		void handleWindowEvent(const SDL_Event& event);
	};
}

#endif //GAME_WINDOWMANAGER_H
