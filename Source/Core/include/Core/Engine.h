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

		bool _cleanedUp = false;
		bool _isDone = false;

		static void ErrorCallback(int code, const char* description);

	public:

		static void initializeReferences(Engine* engine);

		const std::string Name;

		borrowed_ptr<Core::MemoryModule>			MemoryModule;
		borrowed_ptr<Core::Config>					Config;
		borrowed_ptr<Gfx::Renderer>					Renderer;
		borrowed_ptr<Gfx::BufferManager>			BufferManager;
		borrowed_ptr<Resources::MeshManager>		MeshManager;
		borrowed_ptr<Resources::FontManager>		FontManager;
		borrowed_ptr<Resources::TextureManager>		TextureManager;
		borrowed_ptr<Resources::MaterialManager>	MaterialManager;
		borrowed_ptr<Resources::ShaderManager>		ShaderManager;
		borrowed_ptr<Core::WindowManager>			WindowManager;
		borrowed_ptr<Core::InputManager>			InputManager;

		explicit Engine(std::string name, std::size_t memorySize);

		virtual ~Engine() { CleanUp(); }

		// Create Window based on current configuration
		auto CreateWindow() const noexcept -> Utils::Handle<Window>;

		// Load configuration
		auto LoadConfig(std::string path = "Config.json") -> bool;

		// Initialize graphics context based on current config
		auto Initialize() -> bool;

		// Change active scene
		auto SwitchScene(borrowed_ptr<Logic::Scene> scene) -> void;

		// Draw active scene
		auto Draw(const Core::Window& window, Core::GameTime& time) -> void;

		// Update active scene
		auto Update(Core::GameTime& time) -> void;

		// Return current time in ms
		auto GetCurrentTime() -> double;

		// Process user input
		auto ProcessEvents(Core::GameTime& time) -> void;

		// Free all resources
		auto CleanUp() -> void;

		// Call this when user wants to quit
		auto Quit() -> void;

		bool WasCleanedUp();

		bool ShouldClose();

		template <typename T>
		T* CreateManager(std::size_t memorySize)
		{
			static_assert(std::is_base_of<IManager, T>::value, "Engine : Cannot create manager that doesnt derive from IManager!");
			Memory::IMemoryBlock& memoryBlock = MemoryModule.get().requestMemoryBlock(memorySize, T::GetStaticClassName());
			T* manager = YAGE_CREATE_NEW(MemoryModule.get().masterBlock(), T)(*this, memoryBlock);

			return manager;
		}
	};
}

#endif //YAGE_ENGINE_H
