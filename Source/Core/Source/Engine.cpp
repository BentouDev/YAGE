//
// Created by mrjaqbq on 07.03.16.
//

#include <algorithm>
#include <TypeInfo.h>

#include "Resources/Mesh/MeshManager.h"
#include "Resources/ResourceManager.h"
#include "Gfx/Api/BaseApi.h"
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
		  Console(new Core::Console()),
		  MeshManager(new Resources::MeshManager(*this))
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

	auto Engine::GetApi() const noexcept -> Gfx::BaseApi&
	{
		return *_api;
	}

	template <typename Api>
	auto Engine::RegisterApi() -> void
	{
		auto api = new Api();
		_availableApis[api->name()] = borrowed_ptr<Gfx::BaseApi>(api);
		Logger->Default->info("Found {} renderer...", api->name());
	}

	auto Engine::CreateWindow() const noexcept -> Window&
	{
		// todo: save that goddamn pointer somewhere to be reachable!
		// todo: also replace raw ptr with nice handler
		// todo: use container with new, specialized for window trait, which depends on _api
		// todo: _api may leak resources if we want to free them like we are doing now (in engine methods)!
		// todo: pass _api to context!
		Window* window = new Window(GetContext());

		_api->registerWindow(*window);

		return *window;
	}

	auto Engine::LoadConfig(std::string path) -> bool
	{
		return Config->Load(path);
	}

	auto Engine::Initialize(borrowed_ptr<Gfx::BaseApi> api) -> bool
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

	// todo: remove window from here
	auto Engine::Draw(const Core::Window& window) -> void
	{
		// todo: this should be connected to rendertarget
		_api->beginDraw(window);

		// activeScene->Draw(GameTime(), (*Renderer));
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
		if(_cleanedUp)
			return;

		_cleanedUp = true;

		Logger->Default->info("Cleaning up...");

		if(_api)
		{
			_api->cleanUp();
			_api.release();
		}

		for(auto api : _availableApis)
		{
			Memory::SafeDelete(api.second);
		}

		Memory::SafeDelete(MeshManager);
		Memory::SafeDelete(Console);
		Memory::SafeDelete(Renderer);
		Memory::SafeDelete(Config);
		Memory::SafeDelete(Logger);

		glfwTerminate();
	}
}
