//
// Created by mrjaqbq on 07.03.16.
//

#include <algorithm>

#include "Resources/ResourceManager.h"
#include "Gfx/Api/BaseDevice.h"
#include "Gfx/Renderer.h"
#include "Logic/Scene.h"
#include "Platform.h"
#include "Console.h"
#include "Engine.h"
#include "Window.h"
#include "Logger.h"
#include "Config.h"

#define CreateWindow
#undef CreateWindow

#ifdef VOLKHVY_VULKAN
#include "Gfx/Vulkan/VulkanDevice.h"
#endif

#ifdef VOLKHVY_OPENGL
#include "Gfx/OpenGl/OpenGlContext.h"
#endif

namespace Core
{
	void Engine::initializeReferences(Engine* engine)
	{
		engine->Logger->Default->info("Initializing Volkhvy for '{}'...", engine->Name);

#ifdef VOLKHVY_VULKAN
		engine->RegisterApi<Gfx::VulkanDevice>();
#endif

#ifdef VOLKHVY_OPENGL
		engine->RegisterApi<Gfx::OpenGlContext>();
#endif
	}

	Engine::Engine(std::string name)
		: Name(name),
		  Logger(new Core::Logger()),
		  Config(new Core::Config()),
		  Renderer(new Gfx::Renderer()),
		  Console(new Core::Console())
	{
		Logger->setConfig(Config);
		Config->setLogger(Logger);
		Engine::initializeReferences(this);
	}

	auto Engine::GetContext() const noexcept -> Context
	{
		Context ctx(Config, Logger);
		return ctx;
	}

	template <typename Api>
	auto Engine::RegisterApi() -> void
	{
		auto api = new Api();
		_availableApis[api->name()] = borrowed_ptr<Gfx::BaseDevice>(api);
		Logger->Default->info("Found {} renderer...", api->name());
	}

	auto Engine::CreateWindow() const noexcept -> Window&
	{
		Window* window = new Window(GetContext());

		_api->registerWindow(*window);

		return *window;
	}

	auto Engine::LoadConfig(std::string path) -> bool
	{
		return Config->Load(path);
	}

	auto Engine::Initialize(borrowed_ptr<Gfx::BaseDevice> api) -> bool
	{
		if(api)
		{
			_api = api;
		}
		else
		{
			std::string requestedApi = Config->RenderingApi;
			std::transform(requestedApi.begin(), requestedApi.end(), requestedApi.begin(), ::tolower);

			auto itr = _availableApis.find(requestedApi);
			if(itr != _availableApis.end())
			{
				_api = itr->second;
			}
		}

		if(!_api)
		{
			return false;
		}
		else
		{
			return InitializeApi();
		}
	}

	auto Engine::SwitchScene(borrowed_ptr<Logic::Scene> scene) -> void
	{
		// todo: refactor to handle instead of raw pointer
		if(activeScene) activeScene->End();

		activeScene = scene;

		if(activeScene) activeScene->Start();
	}

	auto Engine::InitializeApi() -> bool
	{
		return _api->initialize();
	}

	// todo: remove this
	auto Engine::Draw(const Core::Window& window) -> void
	{
		_api->beginDraw(window);
		// todo: Synchronize commandlists with queue
		/*for(RenderPass pass : _renderPasses)
		{
			Renderer.Draw(pass);
		}*/
		_api->endDraw(window);
	}

	auto Engine::ProcessEvents() -> void
	{
		glfwPollEvents();
	}

	auto Engine::Resize(const Window& window) -> void
	{
		_api->resizeWindow(window);
	}

	auto Engine::CleanUp() -> void
	{
		Logger->Default->info("Cleaning up...");
		_api->cleanUp();
		glfwTerminate();
	}
}