//
// Created by mrjaqbq on 07.03.16.
//

#include <algorithm>
#include <Utils/TypeInfo.h>
#include <Utils/MemorySizes.h>

#include "Core/EngineApis.h"
#include "Core/MemoryModule.h"
#include "Core/WindowManager.h"
#include "Core/Input/InputManager.h"
#include "Core/Resources/ResourceManager.h"
#include "Core/Resources/Mesh/MeshManager.h"
#include "Core/Resources/Shader/ShaderManager.h"
#include "Core/Resources/Texture/TextureManager.h"
#include "Core/Resources/Material/MaterialManager.h"
#include "Core/Gfx/BufferManager.h"
#include "Core/Gfx/Renderer.h"
#include "Core/Logic/Scene.h"
#include "Core/EventQueue.h"
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
		: Name(name), _isDone(false), _cleanedUp(false)
	{
		MemoryModule	.reset(new Core::MemoryModule(memorySize));
		Config			.reset(new Core::Config(
			MemoryModule.get().requestMemoryBlock(Memory::KB(1), "Config Block"))
		);

		Renderer		.reset(CreateManager<Gfx::Renderer>             (Memory::KB(100)));
		BufferManager	.reset(CreateManager<Gfx::BufferManager>        (Memory::KB(100)));
		MeshManager		.reset(CreateManager<Resources::MeshManager>    (Memory::KB(100)));
		TextureManager	.reset(CreateManager<Resources::TextureManager> (Memory::MB(4)));
		MaterialManager	.reset(CreateManager<Resources::MaterialManager>(Memory::KB(100)));
		ShaderManager	.reset(CreateManager<Resources::ShaderManager>  (Memory::KB(100)));
		WindowManager	.reset(CreateManager<Core::WindowManager>       (Memory::KB(10)));
		InputManager	.reset(CreateManager<Core::InputManager>        (Memory::KB(10)));
	}

	auto Engine::CreateWindow() const noexcept -> Window::handle_t
	{
		auto handle = WindowManager.get().createNew (
			((std::string)Config.get().WindowTitle).c_str(),
			 Config.get().WindowWidth,
			 Config.get().WindowHeight
		);

		auto* window = WindowManager.get().tryGet(handle);
		if(window != nullptr)
		{
			if(!OpenGL::registerWindow(*window))
			{
				Logger::error("Engine : Unable to register window in OpenGL!");
			}

			if(!EventQueue::registerWindow(window))
			{
				Logger::error("Engine : Unable to register window in EventQueue!");
			}
		}
		else
		{
			Logger::error("Unable to create window!");
		}

		return handle;
	}

	auto Engine::LoadConfig(std::string path) -> bool
	{
		return Config->Load(path);
	}

	auto Engine::Initialize() -> bool
	{
		bool success = true;
		success &= OpenGL::initialize();
		success &= EventQueue::initialize(MemoryModule->requestMemoryBlock(Memory::KB(200), "EventQueueBlock"));
		return success;
	}

	auto Engine::SwitchScene(borrowed_ptr<Logic::Scene> scene) -> void
	{
		// todo: refactor to handle instead of raw pointer
		if(activeScene) activeScene->End();

		activeScene = scene;

		if(activeScene) activeScene->Start();
	}

	// todo: remove window from here
	auto Engine::Draw(const Core::Window& window, Core::GameTime& time) -> void
	{
		OpenGL::beginDraw(window);

		if(activeScene) activeScene->Draw(time, Renderer.get());

		Renderer->draw();

		OpenGL::endDraw(window);
	}

	auto Engine::ProcessEvents(Core::GameTime& time) -> void
	{
		if(ShouldClose())
			return;

		Event event;
		while(EventQueue::pollEvent(&event))
		{
			switch(event.type)
			{
				case EventType::WINDOW:
					WindowManager->handleWindowEvent(event);
					break;
				case EventType::INPUT:
					InputManager->handleInputEvent(event, time);
					break;
				case EventType::APP:
					break;
			}
		}
	}

	void Engine::Update(GameTime& time)
	{
		if(activeScene)
			activeScene->Update(time);

		if(WindowManager->allWindowsClosed())
			Quit();
	}

	double Engine::GetCurrentTime()
	{
		return glfwGetTime();
	}

	auto Engine::Quit() -> void
	{
		WindowManager->closeAllWindows();
		_isDone = true;
	}

	auto Engine::CleanUp() -> void
	{
		if(_cleanedUp)
			return;

		Logger::info("Cleaning up...");

		EventQueue::destroy();

		Memory::Delete(MemoryModule->masterBlock(), InputManager);
		Memory::Delete(MemoryModule->masterBlock(), WindowManager);
		Memory::Delete(MemoryModule->masterBlock(), BufferManager);
		Memory::Delete(MemoryModule->masterBlock(), MeshManager);
		Memory::Delete(MemoryModule->masterBlock(), TextureManager);
		Memory::Delete(MemoryModule->masterBlock(), MaterialManager);
		Memory::Delete(MemoryModule->masterBlock(), ShaderManager);
		Memory::Delete(MemoryModule->masterBlock(), Renderer);

		glfwTerminate();

		Logger::info("Cleaned up!");
		Memory::SafeDelete(Config);
		Memory::SafeDelete(MemoryModule);

		_cleanedUp = true;
	}

	bool Engine::WasCleanedUp()
	{
		return _cleanedUp;
	}

	bool Engine::ShouldClose()
	{
		return _isDone || WasCleanedUp();
	}
}
