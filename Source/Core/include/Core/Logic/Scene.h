//
// Created by mrjaqbq on 17.04.16.
//

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <Utils/String.h>
#include <cassert>

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
	class World;
	class RenderingSystem;

	class Scene
	{
	protected:

		Memory::IMemoryBlock& _memory;

		const char* Name;

		Core::Engine& engine;

		Logic::World* world;

		// Scene may have list of Entities
		// So when its unloaded from Game/World it may try to free all of them :)

	public:
		explicit Scene(Core::Engine& engine, Memory::IMemoryBlock& memory, const char* name);

		virtual ~Scene();

		virtual void Update(const Core::GameTime& time);

		virtual void Draw(const Core::GameTime& time, Gfx::Renderer& renderer);

		virtual void Start();

		virtual void End();

		inline void setViewport(Gfx::Viewport* viewport)
		{ defaultViewport = viewport; }

		void setWorld(World* newWorld);

		inline World& getWorld() const
		{
			assert(world != nullptr && "World in scene cannot be nullptr!");
			return *world;
		}

		Gfx::Viewport*	defaultViewport;
		Gfx::Camera*	defaultCamera;
	};
}

#endif //GAME_SCENE_H
