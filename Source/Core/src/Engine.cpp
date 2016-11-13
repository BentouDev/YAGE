//
// Created by mrjaqbq on 07.03.16.
//

#include <algorithm>
#include <Utils/TypeInfo.h>

#include "Core/EngineApis.h"

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
	Engine::Engine(std::string name, Memory::IMemoryBlock& blah)
		: Name(name),
		  Logger(new Core::Logger()),
		  Config(new Core::Config(blah))
	//	  Renderer(new Gfx::Renderer())
	{
		Logger->setConfig(Config);
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

	//	_api->registerWindow(*window);

        if(!OpenGL::registerWindow(*window))
		{
			Logger->Default->error("Unable to register window in Gfx Api!");
		}

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
			OpenGL::initialize();
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
	//	return _api->initialize();
		return true;
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

		Logger->Default->info("Cleaning up...");

	//	if(_api)
	//	{
	//		_api->cleanUp();
	//		_api.release();
	//	}

	//	for(auto api : _availableApis)
	//	{
	//		Memory::SafeDelete(api.second);
	//	}

		Memory::SafeDelete(BufferManager);
		Memory::SafeDelete(MeshManager);
		Memory::SafeDelete(MaterialManager);
		Memory::SafeDelete(ShaderManager);
		Memory::SafeDelete(Renderer);
		Memory::SafeDelete(Config);

		glfwTerminate();

		Logger->Default->info("Cleaned up!");
		Memory::SafeDelete(Logger);
	}

	void Engine::debugSetRenderer(Gfx::Renderer* renderer)
	{
		Renderer.reset(renderer);
	}

	void Engine::debugSetBufferManager(Gfx::BufferManager* bufferManager)
	{
		BufferManager.reset(bufferManager);
	}

	void Engine::debugSetMeshManager(Resources::MeshManager* manager)
	{
		MeshManager.reset(manager);
	}

	void Engine::debugSetMaterialManager(Resources::MaterialManager* manager)
	{
		MaterialManager.reset(manager);
	}

	void Engine::debugSetShaderManager(Resources::ShaderManager* manager)
	{
		ShaderManager.reset(manager);
	}
}
