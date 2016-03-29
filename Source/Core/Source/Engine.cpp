//
// Created by mrjaqbq on 07.03.16.
//

#include <spdlog/spdlog.h>
#include "Resources/ResourceManager.h"
#include "Gfx/Vulkan/VulkanDevice.h"
#include "Gfx/OpenGl/OpenGlContext.h"
#include "Gfx/Renderer.h"
#include "Engine.h"
#include "Window.h"

namespace Core
{
	Engine::Engine(std::string name) :
			_api { new Gfx::OpenGlContext() }
	{
		namespace spd = spdlog;

		auto console = spd::stdout_logger_mt("console");
		console->info("Initializing Volkhvy for '{}'...", name);
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

	auto Engine::Initialize() -> bool
	{
		if(!glfwInit())
		{
			return false;
		}

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