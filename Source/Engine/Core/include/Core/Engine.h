//
// Created by mrjaqbq on 07.03.16.
//

#ifndef YAGE_ENGINE_H
#define YAGE_ENGINE_H

#include <memory>
#include <string>
#include <unordered_map>

#include <Utils/Defines.h>
#include <Utils/Handle.h>
#include <Utils/List.h>
#include <Utils/OwnedPtr.h>

#include "MemoryModule.h"

namespace Logic
{
	class Scene;

	class SceneManager;
}

namespace Gfx
{
	class Renderer;
	class BufferManager;
}

namespace Resources
{
	class MeshManager;
	class FontManager;
	class TextureManager;
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

	class WindowManager;

	class MessageDispather;

	class InputManager;

	struct Context;

	struct GameTime;

	class IManager;

	class Engine
	{
		borrowed_ptr<Logic::Scene> activeScene;

		List<borrowed_ptr<IManager>> managers;

		bool _cleanedUp = false;
		bool _isDone = false;

		static void ErrorCallback(int code, const char* description);

		void ReleaseManagers();

	public:

		static void initializeReferences(Engine* engine);

		Utils::String Name;

		owned_ptr<Core::MemoryModule>			MemoryModule;
		owned_ptr<Core::Config>					Config;
		owned_ptr<Gfx::Renderer>				Renderer;
		owned_ptr<Gfx::BufferManager>			BufferManager;
		owned_ptr<Resources::MeshManager>		MeshManager;
		owned_ptr<Resources::FontManager>		FontManager;
		owned_ptr<Resources::TextureManager>	TextureManager;
		owned_ptr<Resources::MaterialManager>	MaterialManager;
		owned_ptr<Resources::ShaderManager>		ShaderManager;
		owned_ptr<Core::WindowManager>			WindowManager;
		owned_ptr<Core::InputManager>			InputManager;

		explicit Engine(const char* name, std::size_t memorySize);

		virtual ~Engine();

		// Create Window based on current configuration
		auto CreateWindow() const noexcept -> Utils::Handle<Window>;

		// Load configuration
		bool LoadConfig(const char* = "Config.json");

		// Initialize graphics context based on current config
		bool Initialize();

		// Change active scene
		void SwitchScene(borrowed_ptr<Logic::Scene> scene);

		// Draw active scene
		void Draw(const Core::Window& window, Core::GameTime& time);

		// Update active scene
		void Update(Core::GameTime& time);

		// Return current time in ms
		auto GetCurrentTime() -> double;

		// Process user input
		void ProcessEvents(Core::GameTime& time);

		// Free all resources
		void CleanUp();

		// Call this when user wants to quit
		void Quit();

		bool WasCleanedUp();

		bool ShouldClose();

		template <typename T>
		void CreateManager(owned_ptr<T>& ptr, std::size_t memorySize)
		{
			static_assert(std::is_base_of<IManager, T>::value,
				"Engine : Cannot create manager that doesnt derive from IManager!");

			Memory::IMemoryBlock& memoryBlock = MemoryModule->requestMemoryBlock(memorySize, T::GetStaticClassName());
			T* manager = YAGE_CREATE_NEW(MemoryModule->masterBlock(), T)(*this, memoryBlock);

			ptr.reset(manager);
			managers.add(ptr.template borrow<IManager>());
		}
	};
}

#endif //YAGE_ENGINE_H
