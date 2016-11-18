//
// Created by mrjaqbq on 07.03.16.
//

#ifndef YAGE_ENGINE_H
#define YAGE_ENGINE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <Utils/Handle.h>
#include <Utils/List.h>

#include "Core/Gfx/Renderer.h"
#include "MemoryModule.h"
#include "Context.h"

#ifdef CREATE_NEW
#undef CREATE_NEW
#endif

#ifdef CreateWindow
#undef CreateWindow
#endif

namespace Logic
{
	class Scene;

	class SceneManager;
}

namespace Gfx
{
	class BufferManager;
	class BaseApi
	{};
}

namespace Resources
{
	class MeshManager;
	class MaterialManager;
	class ShaderManager;
}

namespace Scripts
{
	class ScriptManager;
}

namespace Core
{
	using namespace Utils;

	class Config;

	class Logger;

	class Window;

	struct Context;

	struct GameTime;

	class IManager;

	class Engine
	{
		borrowed_ptr<Logic::Scene> activeScene;

		bool _cleanedUp = false;

	public:

		static void initializeReferences(Engine* engine);

		const std::string Name;

		borrowed_ptr<MemoryModule> MemoryModule;

		borrowed_ptr<Core::Logger> Logger;

		borrowed_ptr<Core::Config> Config;

		borrowed_ptr<Gfx::Renderer> Renderer;

		borrowed_ptr<Gfx::BufferManager> BufferManager;

		borrowed_ptr<Resources::MeshManager> MeshManager;
		borrowed_ptr<Resources::MaterialManager> MaterialManager;
		borrowed_ptr<Resources::ShaderManager> ShaderManager;

		explicit Engine(std::string name, std::size_t memorySize);

		virtual ~Engine() { CleanUp(); }

		// Create Window based on current configuration
		auto CreateWindow() const noexcept -> Window&;

		// Load configuration
		auto LoadConfig(std::string path = "Config.json") -> bool;

		// Initialize graphics context based on current config
		auto Initialize() -> bool;

		auto SwitchScene(borrowed_ptr<Logic::Scene> scene) -> void;

		// Draw all renderpasses for active scenes
		auto Draw(const Core::Window& window) -> void;

		// todo: each window should have queue of events to process
		// Process user input
		auto ProcessEvents() -> void;

		// Resize window
		auto Resize(const Window& window) -> void;

		// Free all resources
		auto CleanUp() -> void;

		template <typename T>
		T* CreateManager(std::size_t memorySize)
		{
			Memory::IMemoryBlock& memoryBlock = MemoryModule.get().requestMemoryBlock(memorySize, TypeInfo<T>::cName());
			T* manager = YAGE_CREATE_NEW(MemoryModule.get().masterBlock(), T)(*this, memoryBlock);

			return manager;
		}

		// todo: Decide what to do next based on config
		// todo: Render all
		// todo: Gather all input
		// todo: Threads, etc.
	};
}

#endif //YAGE_ENGINE_H
