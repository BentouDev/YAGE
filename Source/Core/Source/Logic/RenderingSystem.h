//
// Created by bentoo on 10/14/16.
//

#ifndef GAME_RENDERINGSYSTEM_H
#define GAME_RENDERINGSYSTEM_H

#include <cstdint>
#include "Component.h"

#include <Utils/DefaultTrait.h>
#include <Utils/Container.h>

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
	class Mesh;
	class Material;
}

namespace Logic
{
	DECL_COMP(RenderingComponent)
	{
		bool					_isVisible;
		bool 					_isDirty;

		// TODO: use handles instead of pointers!
	//	Core::Mesh::handle_t	_mesh;
		Core::Mesh*				_debugMesh;
		Utils::List<Core::Material*> _debugMaterials;

	public:
		inline explicit RenderingComponent(Memory::IMemoryBlock& memory) : _debugMaterials(memory)
		{

		}

		RenderingComponent(const RenderingComponent&) = delete;

		~RenderingComponent()
		{

		}

		auto swap(RenderingComponent& other) noexcept -> void override
		{
			assert(false && "SWAP IN RENDERCOMP NOT IMPLEMENTED");
		}

		auto cleanUp() noexcept -> void override
		{

		}

		inline bool isDirty() const
		{ return _isDirty; }

		inline bool isVisible() const
		{ return _isVisible; }

		inline void hide()
		{ setVisibility(false); }

		inline void show()
		{ setVisibility(true); }

		inline void setVisibility(bool visible)
		{
			if(_isVisible != visible)
			{
				_isVisible = visible;
				_isDirty = true;
			}
		}

		void setMaterial(Core::Material& material, std::uint32_t index);

		inline Core::Material& getMaterial(std::uint32_t index) const
		{
			// TODO: PROPER ERROR HANDELING
			assert(index < _debugMaterials.size() && "Material index out of bounds");
			return *_debugMaterials[index];
		}

		inline void setMesh(Core::Mesh& mesh)
		{ _debugMesh = &mesh; }

		inline Core::Mesh& getMesh() const
		{ return *_debugMesh; }
	};

	class RenderingComponentTrait : public Utils::DefaultTrait<RenderingComponent> { };

	class RenderingSystem
	{
		Core::Engine& _engine;
		Memory::IMemoryBlock& _memory;
		Utils::Container<RenderingComponentTrait> _components;

		void uploadMeshToBucket(Gfx::Renderer& renderer, Core::Mesh& mesh);
		void updateMeshInBucket(Gfx::Renderer& renderer, Core::Mesh& mesh);

	//	Utils::List<RenderingComponent*> _dirtyComponents;
	//	Utils::List<Gfx::RenderBatchTrait::handle> _dirtyBatches;

	public:
		explicit RenderingSystem(Core::Engine& engine, Memory::IMemoryBlock& memory);

		auto createNew() -> RenderingComponent::handle_t;
		auto remove(RenderingComponent::handle_t handle) -> void;
		auto update(const Core::GameTime& time, Gfx::Renderer& renderer) -> void;
		RenderingComponent& get(RenderingComponent::handle_t) const;
	};
}

#endif //GAME_RENDERINGSYSTEM_H
