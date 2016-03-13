//
// Created by mrjaqbq on 07.03.16.
//

#include <Gfx/Api/VulkanApi.h>
#include "Engine.h"

namespace Core
{
	Engine::Engine() : _api { new Gfx::VulkanApi() }
	{

	}

	auto Engine::CreateWindow() const noexcept -> std::unique_ptr<Window>
	{
		auto window = std::unique_ptr<Window>(new Window());

		_api->registerWindow(window->hWindow);

		return window;
	}

	auto Engine::LoadConfig(std::string path) -> bool
	{

	}

	auto Engine::Initialize() -> bool
	{
		if(!glfwInit())
		{
			return false;
		}

		InitializeApi();

		return true;
	}

	auto Engine::InitializeApi() -> bool
	{
		_api->initialize();
	}

	auto Engine::Draw() -> void
	{
		// todo: Synchronize commandlists with queue
		/*for(RenderPass pass : _renderPasses)
		{
			Renderer.Draw(pass);
		}*/
	}

	auto Engine::ProcessEvents() -> void
	{
		glfwPollEvents();
	}

	auto Engine::CleanUp() -> void
	{
		_api->cleanUp();
		glfwTerminate();
	}
}