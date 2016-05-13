//
// Created by mrjaqbq on 07.03.16.
//

#ifndef VOLKHVY_ENGINE_H
#define VOLKHVY_ENGINE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <Handle.h>
#include "Context.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

namespace Logic
{
	class Scene;

	class SceneManager;
}

namespace Gfx
{
	class BaseDevice;

	class Renderer;
}

namespace Scripts
{
	class ScriptManager;
}

namespace Core
{
	using namespace Utils;

	class Config;

	class Logger;

	class Console;

	class Window;

//	class ResourceManager;

	struct Context;

	struct GameTime;

	class Engine
	{
		std::unordered_map<std::string, borrowed_ptr<Gfx::BaseDevice>> _availableApis;

		borrowed_ptr<Gfx::BaseDevice> _api;

		auto InitializeApi() -> bool;

		borrowed_ptr<Logic::Scene> activeScene;

	public:

		static void initializeReferences(Engine* engine);

		const std::string Name;

		borrowed_ptr<Core::Config> Config;

		borrowed_ptr<Gfx::Renderer> Renderer;

		borrowed_ptr<Core::Logger> Logger;

		borrowed_ptr<Core::Console> Console;

		explicit Engine(std::string name);

		auto GetContext() const noexcept  -> Context;

		template <typename Api>
		auto RegisterApi() -> void;

		// Create Window based on current configuration
		auto CreateWindow() const noexcept -> Window&;

		// Load configuration
		auto LoadConfig(std::string path = "Config.json") -> bool;

		// Initialize graphics context based on current config
		auto Initialize(borrowed_ptr<Gfx::BaseDevice> api = borrowed_ptr<Gfx::BaseDevice>()) -> bool;

		auto SwitchScene(borrowed_ptr<Logic::Scene> scene) -> void;

		// Draw all renderpasses for active scenes
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
