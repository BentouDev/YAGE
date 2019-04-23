//
// Created by bentoo on 10/14/16.
//

#ifndef GAME_RENDERINGSYSTEM_H
#define GAME_RENDERINGSYSTEM_H

#include "RenderingComponent.h"
#include "Core/Logic/System.h"

namespace Gfx
{
	class Renderer;
}

namespace Memory
{
	class IMemoryBlock;
}

namespace Core
{
	class Engine;
	struct GameTime;
}

namespace Logic
{
	class World;

	class RenderingSystem : public System<RenderingSystem, Requires<RenderingComponent>>
	{
		friend class RenderingComponent;

		Core::Engine&			_engine;
		Memory::IMemoryBlock&	_memory;
		World&					_world;

		Utils::List<RenderingComponent::handle_t> _dirtyComponents;

		void refreshDirtyComponents();
		void refreshDirtyComponent(RenderingComponent&);
		void createVAO(RenderingComponent&);

		struct SceneInfo
		{
			SceneInfo(Scene* ptr, Memory::IMemoryBlock& memory, Entity::handle_t handle)
				: scene(ptr), entities() // #NewAlloc
			{
				entities.push_back(handle);
			}

			Scene*							scene;
			Utils::List<Entity::handle_t>	entities;
		};

		Utils::List<SceneInfo> _sceneEntities;

	public:
		explicit RenderingSystem(Core::Engine& engine, Memory::IMemoryBlock& memory, World& world);

		auto setDirty(RenderingComponent& comp) -> void;
		void update(const Core::GameTime& time) override;
		void addEntity(entity_handle_t handle) override;
	};
}

#endif //GAME_RENDERINGSYSTEM_H
