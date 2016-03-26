//
// Created by mrjaqbq on 07.03.16.
//

#ifndef VOLKHVY_ENGINE_H
#define VOLKHVY_ENGINE_H

#include <memory>
#include <string>

#ifdef CreateWindow
#undef CreateWindow
#endif

namespace Gfx
{
	class BaseDevice;
}

namespace Core
{
	class Window;

	class Engine
	{
		// todo: reference to config
		// todo: config object passed as a parameter in constructor
		// todo: config object created in base constructor

		Gfx::BaseDevice* _api;

		auto InitializeApi() -> bool;

	public:
		explicit Engine(std::string name);

		// Create Window based on current configuration
		auto CreateWindow() const noexcept -> Window&;

		// Load configuration
		auto LoadConfig(std::string path) -> bool;

		// Initialize graphics context based on current config
		auto Initialize() -> bool;


		// Draw all renderpasses
		auto Draw(const Core::Window& window) -> void;

		// todo: each window should have queue of events to process
		// Process user input
		auto ProcessEvents() -> void;

		// Resize window
		auto Resize(const Window& window) -> void;

		// Free all resources
		auto CleanUp() -> void;

		// todo: Decide what to do next basing on config
		// todo: Render all
		// todo: Gather all input
		// todo: Threads, etc.
	};
}

#endif //VOLKHVY_ENGINE_H
