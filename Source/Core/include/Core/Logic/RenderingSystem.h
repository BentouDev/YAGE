//
// Created by bentoo on 10/14/16.
//

#ifndef GAME_RENDERINGSYSTEM_H
#define GAME_RENDERINGSYSTEM_H

#include <Utils/Container.h>
#include <Core/Gfx/Camera.h>
#include <Core/Gfx/Viewport.h>

#include "RenderingComponent.h"

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
	class GameTime;
}

namespace Logic
{
	class RenderingSystem
	{
		friend class RenderingComponent;

		Core::Engine& _engine;
		Memory::IMemoryBlock& _memory;

		Utils::Container<RenderingComponentTrait> _components;
		Utils::List<RenderingComponent::handle_t> _dirtyComponents;

		void refreshDirtyComponents();
		void createVAO(RenderingComponent&);

	public:
		explicit RenderingSystem(Core::Engine& engine, Memory::IMemoryBlock& memory);

		auto setDirty(RenderingComponent& comp) -> void;
		auto createNew() -> RenderingComponent::handle_t;
		auto remove(RenderingComponent::handle_t handle) -> void;
		void
		update(const Core::GameTime &time, Gfx::Renderer &renderer, Gfx::Camera *pCamera, Gfx::Viewport *pViewport);
		RenderingComponent& get(RenderingComponent::handle_t) const;
	};
}

#endif //GAME_RENDERINGSYSTEM_H
