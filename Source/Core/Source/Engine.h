//
// Created by mrjaqbq on 07.03.16.
//

#ifndef VOLKHVY_ENGINE_H
#define VOLKHVY_ENGINE_H

#include <memory>
#include <string>
#include "Gfx/Api/VulkanApi.h"
#include "Window.h"

namespace Core
{
	class Engine
	{
		// todo: reference to config
		// todo: config object passed as a parameter in constructor
		// todo: config object created in base constructor

		Gfx::VulkanApi* _api;

		auto InitializeApi() -> bool;

	public:
		explicit Engine();

		// Create Window based on current configuration
		auto CreateWindow() const noexcept -> std::unique_ptr<Window>;

		// Load configuration
		auto LoadConfig(std::string path) -> bool;

		// Initialize graphics context based on current config
		auto Initialize() -> bool;

		// Free all resources
		auto CleanUp() -> void;

		// Draw all renderpasses
		auto Draw() -> void;

		// todo: each window should have queue of events to process
		// Process user input
		auto ProcessEvents() -> void;

		// todo: Decide what to do next basing on config
		// todo: Render all
		// todo: Gather all input
		// todo: Threads, etc.
	};
}

#endif //VOLKHVY_ENGINE_H
