//
// Created by bentoo on 10/14/16.
//

#ifndef GAME_RENDERINGSYSTEM_H
#define GAME_RENDERINGSYSTEM_H

#include <cstdint>
#include "Component.h"

#include <Utils/DefaultTrait.h>
#include <Utils/Container.h>
#include "../Gfx/BatchManager.h"

namespace Gfx
{
	class Renderer;
	class MaterialBatch;
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
	class RenderingSystem;

	DECL_COMP(RenderingComponent)
	{
		friend class Gfx::Renderer;
		friend class RenderingSystem;

		RenderingSystem& _system;

		bool _isVisible;
		bool _isDirty;

		Utils::Handle<Core::Mesh>						_mesh;
		Utils::List<Utils::Handle<Core::Material>>		_materials;
		Utils::List<Gfx::MaterialBatchMeshReference>	_batchHandles;

	public:
		inline explicit RenderingComponent(RenderingSystem& system, Memory::IMemoryBlock& memory)
			: _system(system), _materials(memory), _batchHandles(memory),
			  _isDirty(false), _isVisible(true)
		{
			setDirty();
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

		void setDirty();

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
				setDirty();
			}
		}

		void setMaterial(Core::Material& material, std::uint32_t index);

		inline Utils::Handle<Core::Material> getMaterial(std::uint32_t index) const
		{
			// TODO: PROPER ERROR HANDLING
			assert(index < _materials.size() && "Material index out of bounds");
			return _materials[index];
		}

		inline const Utils::List<Utils::Handle<Core::Material>>& getMaterials() const
		{ return _materials; }

		inline void setMesh(Utils::Handle<Core::Mesh> handle)
		{ _mesh = handle; }

		inline Utils::Handle<Core::Mesh> getMesh() const
		{ return _mesh; }
	};

	class RenderingComponentTrait : public Utils::DefaultTrait<RenderingComponent> { };

	class RenderingSystem
	{
		Core::Engine& _engine;
		Memory::IMemoryBlock& _memory;
		Utils::Container<RenderingComponentTrait> _components;

		void uploadMeshToBucket(Gfx::Renderer& renderer, Core::Mesh& mesh);
		void updateMeshInBucket(Gfx::Renderer& renderer, Core::Mesh& mesh);

		Utils::List<RenderingComponent::handle_t> _dirtyComponents;
	//	Utils::List<Gfx::RenderBatchTrait::handle> _dirtyBatches;

		void refreshDirtyComponents();

	public:
		explicit RenderingSystem(Core::Engine& engine, Memory::IMemoryBlock& memory);

		auto setDirty(RenderingComponent& comp) -> void;
		auto createNew() -> RenderingComponent::handle_t;
		auto remove(RenderingComponent::handle_t handle) -> void;
		auto update(const Core::GameTime& time, Gfx::Renderer& renderer) -> void;
		RenderingComponent& get(RenderingComponent::handle_t) const;
	};
}

#endif //GAME_RENDERINGSYSTEM_H
