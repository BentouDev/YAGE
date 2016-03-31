//
// Created by mrjaqbq on 07.03.16.
//

#include "Resources/ResourceManager.h"
#include "Gfx/Api/BaseDevice.h"
#include "Gfx/Renderer.h"
#include "Platform.h"
#include "Engine.h"
#include "Window.h"
#include "Logger.h"

namespace Core
{
	Engine::Engine(std::string name)
	{
		Logger::getInstance().Console->info("Initializing Volkhvy for '{}'...", name);
	}

	auto Engine::CreateWindow() const noexcept -> Window&
	{
		Window* window = new Window();

		_api->registerWindow(*window);

		return *window;
	}

	auto Engine::LoadConfig(std::string path) -> bool
	{
		return true;
	}

	auto Engine::Initialize(Gfx::BaseDevice* api) -> bool
	{
		if(api != nullptr)
		{
			_api = api;
		}
		else
		{
			// get from config
			// but how do I instantiate?
		}

		if(!glfwInit())
		{
			return false;
		}

		// todo: assert macro
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
		_api->cleanUp();
		glfwTerminate();
	}
}