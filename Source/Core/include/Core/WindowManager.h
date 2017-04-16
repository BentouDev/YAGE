//
// Created by bentoo on 18.11.16.
//

#ifndef GAME_WINDOWMANAGER_H
#define GAME_WINDOWMANAGER_H

#include <map>
#include <cstdint>
#include <Utils/MemoryBlock.h>
#include <Utils/Container.h>
#include "Core/Window.h"
#include "Core/IManager.h"

namespace Core
{
	struct Event;
	class Engine;

	class WindowManager : public IManager
	{
		MANAGER(WindowManager);

	public:
		using handle_t = Utils::Handle<Core::Window>;

	protected:
		Utils::Container<Window::trait_t>			_windowContainer;
		std::map<std::uintptr_t, Window::handle_t>	_windowIdMapper;

		Window* getWindowById(std::uintptr_t id);

		void handleWindowEvent(Window* window, const Event& event);

	public:
		explicit WindowManager(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~WindowManager();

		handle_t createNew(const char* name, unsigned width, unsigned height);

		Window& get(handle_t handle)
		{ return _windowContainer.get(handle); }

		Window* tryGet(handle_t);

		void closeAllWindows();
		void handleWindowEvent(const Event& event);
		bool allWindowsClosed() const;
	};
}

#endif //GAME_WINDOWMANAGER_H
