//
// Created by mrjaqbq on 07.03.16.
//

#include <algorithm>
#include <Utils/TypeInfo.h>
#include <Utils/MemorySizes.h>

#include "Core/EngineApis.h"
#include "Core/MemoryModule.h"
#include "Core/WindowManager.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Mesh/MeshManager.h"
#include "Core/Resources/Shader/ShaderManager.h"
#include "Core/Resources/Texture/TextureManager.h"
#include "Core/Resources/Material/MaterialManager.h"
#include "Core/Gfx/BufferManager.h"
#include "Core/Gfx/Renderer.h"
#include "Core/Logic/Scene.h"
#include "Core/GameTime.h"
#include "Core/Engine.h"
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
		TextureManager	.reset(CreateManager<Resources::TextureManager>(Memory::MB(4)));
		MaterialManager	.reset(CreateManager<Resources::MaterialManager>(Memory::KB(100)));
		ShaderManager	.reset(CreateManager<Resources::ShaderManager>(Memory::KB(100)));
		WindowManager	.reset(CreateManager<Core::WindowManager>(Memory::KB(10)));
	}

	auto Engine::CreateWindow() const noexcept -> Window::handle_t
	{
		auto handle = WindowManager.get().createNew(((std::string)Config.get().WindowTitle).c_str(),
													  Config.get().WindowWidth,
													  Config.get().WindowHeight);

		auto* window = WindowManager.get().tryGet(handle);
		if(window != nullptr)
		{
			if(!OpenGL::registerWindow(*window))
			{
				Logger::get()->error("Unable to register window in OpenGL!");
			}
		}
		else
		{
			Logger::get()->error("Unable to create window!");
		}

		return handle;
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
		// TODO: implement proper time management
		static GameTime time;

		OpenGL::beginDraw(window);

		activeScene->Draw(time, Renderer.get());

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

		Memory::Delete(MemoryModule.get().masterBlock(), WindowManager);
		Memory::Delete(MemoryModule.get().masterBlock(), BufferManager);
		Memory::Delete(MemoryModule.get().masterBlock(), MeshManager);
		Memory::Delete(MemoryModule.get().masterBlock(), TextureManager);
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
