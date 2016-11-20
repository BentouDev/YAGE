//
// Created by mrjaqbq on 17.04.16.
//

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <Utils/String.h>

namespace Core
{
	class Engine;
	struct GameTime;
}

namespace Gfx
{
	class Renderer;
	class Camera;
	class Viewport;
}

namespace Logic
{
	class RenderingSystem;

	class Scene
	{
	protected:
		// Reference to script module
		// Containers for entites
		// Containers for entity components

		Memory::IMemoryBlock& _memory;

		const char* Name;

		Core::Engine& engine;

		RenderingSystem* Rendering;

		Gfx::Camera* defaultCamera;
		Gfx::Viewport* defaultViewport;

	public:
		Scene(Core::Engine& engine, Memory::IMemoryBlock& memory, const char* name);

		virtual ~Scene();

		virtual void Update(const Core::GameTime& time);

		// Is this necessary?
		virtual void Draw(const Core::GameTime& time, Gfx::Renderer& renderer);

		virtual void Start();

		virtual void End();

		inline void setViewport(Gfx::Viewport* viewport)
		{ defaultViewport = viewport; }
	};
}

#endif //GAME_SCENE_H
