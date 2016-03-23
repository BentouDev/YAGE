//
// Created by mrjaqbq on 07.03.16.
//

#include <Gfx/Api/VulkanDevice.h>
#include "Engine.h"
#include "Window.h"

namespace Core
{
	Engine::Engine(std::string name) : _api { new Gfx::VulkanDevice() }
	{

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

		InitializeApi();

		return true;
	}

	auto Engine::InitializeApi() -> bool
	{
		return _api->initialize();
	}

	auto Engine::Draw() -> void
	{
		_api->beginDraw();
		// todo: Synchronize commandlists with queue
		/*for(RenderPass pass : _renderPasses)
		{
			Renderer.Draw(pass);
		}*/
		_api->endDraw();
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