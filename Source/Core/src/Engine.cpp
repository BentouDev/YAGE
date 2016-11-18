//
// Created by mrjaqbq on 07.03.16.
//

#include <algorithm>
#include <Utils/TypeInfo.h>
#include <Utils/MemorySizes.h>

#include "Core/EngineApis.h"
#include "Core/MemoryModule.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Mesh/MeshManager.h"
#include "Core/Resources/Shader/ShaderManager.h"
#include "Core/Resources/Material/MaterialManager.h"
#include "Core/Gfx/BufferManager.h"
#include "Core/Gfx/Renderer.h"
#include "Core/Logic/Scene.h"
#include "Core/GameTime.h"
#include "Core/Engine.h"
#include "Core/Window.h"
#include "Core/Logger.h"
#include "Core/Config.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

namespace Core
{
	Engine::Engine(std::string name, std::size_t memorySize)
		: Name(name)
	{
		MemoryModule	.reset(new Core::MemoryModule(memorySize));
		Config			.reset(new Core::Config(MemoryModule.get().requestMemoryBlock(Memory::KB(1), "Config Block")));

		Renderer		.reset(CreateManager<Gfx::Renderer>(Memory::KB(100)));
		BufferManager	.reset(CreateManager<Gfx::BufferManager>(Memory::KB(100)));
		MeshManager		.reset(CreateManager<Resources::MeshManager>(Memory::KB(100)));
		MaterialManager	.reset(CreateManager<Resources::MaterialManager>(Memory::KB(100)));
		ShaderManager	.reset(CreateManager<Resources::ShaderManager>(Memory::KB(100)));
	}

	auto Engine::CreateWindow() const noexcept -> Window&
	{
		// todo: save that goddamn pointer somewhere to be reachable!
		// todo: also replace raw ptr with nice handler
		// todo: use container with new, specialized for window trait, which depends on _api
		// todo: _api may leak resources if we want to free them like we are doing now (in engine methods)!
		// todo: pass _api to context!
		Window* window = new Window(((std::string)Config.get().WindowTitle).c_str(), Config.get().WindowWidth, Config.get().WindowHeight);

		if(!OpenGL::registerWindow(*window))
		{
			Logger::get()->error("Unable to register window in OpenGL!");
		}

		return *window;
	}

	auto Engine::LoadConfig(std::string path) -> bool
	{
		return Config->Load(path);
	}

	auto Engine::Initialize() -> bool
	{
		return OpenGL::initialize();
	}

	auto Engine::SwitchScene(borrowed_ptr<Logic::Scene> scene) -> void
	{
		// todo: refactor to handle instead of raw pointer
		if(activeScene) activeScene->End();

		activeScene = scene;

		if(activeScene) activeScene->Start();
	}

	// todo: remove window from here
	auto Engine::Draw(const Core::Window& window) -> void
	{
		OpenGL::beginDraw(window);

		activeScene->Draw(*new GameTime, Renderer.get());

		Renderer->draw();

		OpenGL::endDraw(window);
	}

	auto Engine::ProcessEvents() -> void
	{
		glfwPollEvents();
	}

	auto Engine::Resize(const Window& window) -> void
	{
		OpenGL::resizeWindow(window);
	}

	auto Engine::CleanUp() -> void
	{
		if(_cleanedUp)
			return;

		_cleanedUp = true;

		Logger::get()->info("Cleaning up...");

		Memory::Delete(MemoryModule->masterBlock(), BufferManager);
		Memory::Delete(MemoryModule.get().masterBlock(), MeshManager);
		Memory::Delete(MemoryModule.get().masterBlock(), MaterialManager);
		Memory::Delete(MemoryModule.get().masterBlock(), ShaderManager);
		Memory::Delete(MemoryModule.get().masterBlock(), Renderer);

		glfwTerminate();

		Logger::get()->info("Cleaned up!");
		Memory::SafeDelete(Config);
		Memory::SafeDelete(MemoryModule);
		Logger::get().destroy();
	}
}
