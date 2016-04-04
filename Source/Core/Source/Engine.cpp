//
// Created by mrjaqbq on 07.03.16.
//

#include <algorithm>

#include "Resources/ResourceManager.h"
#include "Gfx/Api/BaseDevice.h"
#include "Gfx/Renderer.h"
#include "Platform.h"
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
	Engine::Engine(std::string name)
	{
		Logger::get().Console->info("Initializing Volkhvy for '{}'...", name);

#ifdef VOLKHVY_VULKAN
		RegisterApi<Gfx::VulkanDevice>("vulkan");
#endif

#ifdef VOLKHVY_OPENGL
		RegisterApi<Gfx::OpenGlContext>("opengl");
#endif
	}

	template <typename Api>
	auto Engine::RegisterApi(std::string name) -> void
	{
		_availableApis[name] = new Api();
		Logger::get().Console->info("Found {} renderer...", name);
	}

	auto Engine::CreateWindow() const noexcept -> Window&
	{
		Window* window = new Window();

		_api->registerWindow(*window);

		return *window;
	}

	auto Engine::LoadConfig(std::string path) -> bool
	{
		return Config::get().Load(path);
	}

	auto Engine::Initialize(Gfx::BaseDevice* api) -> bool
	{
		if(api != nullptr)
		{
			_api = api;
		}
		else
		{
			std::string requestedApi = Config::get().RenderingApi;
			std::transform(requestedApi.begin(), requestedApi.end(), requestedApi.begin(), ::tolower);

			auto itr = _availableApis.find(requestedApi);
			if(itr != _availableApis.end())
			{
				_api = itr->second;
			}
		}

		// todo: assert macro
		// todo: log errors
		assert(_api);

		return InitializeApi();
	}

	auto Engine::InitializeApi() -> bool
	{
		return _api->initialize();
	}

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
		Logger::get().Console->info("Cleaning up...");
		_api->cleanUp();
		glfwTerminate();
	}
}