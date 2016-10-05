//
// Created by mrjaqbq on 07.03.16.
//

#ifndef VOLKHVY_ENGINE_H
#define VOLKHVY_ENGINE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <Utils/Handle.h>
#include "Gfx/Renderer.h"
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
	class BaseApi
	{};
}

namespace Resources
{
	class MeshManager;
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

	class Window;

//	class ResourceManager;

	struct Context;

	struct GameTime;

	class Engine
	{
		std::unordered_map<std::string, borrowed_ptr<Gfx::BaseApi>> _availableApis;

		borrowed_ptr<Gfx::BaseApi> _api;
		borrowed_ptr<Logic::Scene> activeScene;

		bool _cleanedUp = false;

		auto InitializeApi() -> bool;

	public:

		static void initializeReferences(Engine* engine);

		const std::string Name;

		borrowed_ptr<Core::Config> Config;

		borrowed_ptr<Gfx::Renderer> Renderer;

		borrowed_ptr<Core::Logger> Logger;

	//	borrowed_ptr<Resources::MeshManager> MeshManager;

		explicit Engine(std::string name);

		virtual ~Engine() { CleanUp(); }

		auto GetContext() const noexcept  -> Context;

		auto GetApi() const noexcept -> Gfx::BaseApi&;

		template <typename Api>
		auto RegisterApi() -> void;

		// Create Window based on current configuration
		auto CreateWindow() const noexcept -> Window&;

		// Create renderer of requested type and initalize it internally
		template <typename T, typename... Args>
		auto CreateRenderer(Args... args) -> borrowed_ptr<T>;

		// Load configuration
		auto LoadConfig(std::string path = "Config.json") -> bool;

		// Initialize graphics context based on current config
		auto Initialize(borrowed_ptr<Gfx::BaseApi> api = borrowed_ptr<Gfx::BaseApi>()) -> bool;

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

	template <typename T, typename... Args>
	auto Engine::CreateRenderer(Args... args) -> borrowed_ptr<T>
	{
		borrowed_ptr<T> result;
		if(!Renderer)
		{
			result.reset(new T(args...));
			Renderer.reset(result.getRaw());
			Renderer->initialize(GetContext(), _api);
		}
		return result;
	}
}

#endif //VOLKHVY_ENGINE_H
