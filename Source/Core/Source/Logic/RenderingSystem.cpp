//
// Created by bentoo on 10/14/16.
//

#include <cstdint>
#include <Utils/Container.h>
#include <Gfx/CommandQueue.h>
#include <Engine.h>

#include "RenderingSystem.h"

#include "../Gfx/RenderBatch.h"
#include "../Gfx/Renderer.h"
#include "../Resources/Mesh/Mesh.h"
#include "../Resources/Mesh/MeshManager.h"
#include "../Resources/Material/Material.h"
#include "../Resources/Material/MaterialManager.h"

namespace Logic
{
	void RenderingComponent::setMaterial(Core::Material& material, std::uint32_t index)
	{
		// TODO: protect from creating more materials than mesh supports,
		// or just set size when setting mesh and force list bounds in here!
	//	_debugMaterials.resize(index + 1);
	//	_debugMaterials[index] = material.Handle;
		setDirty();
	}

	void RenderingComponent::setDirty()
	{
		_system.setDirty(*this);
	}

	RenderingSystem::RenderingSystem(Core::Engine &engine, Memory::IMemoryBlock& memory)
		: _engine(engine), _memory(memory), _components(memory), _dirtyComponents(memory)
	{

	}

	void RenderingSystem::setDirty(RenderingComponent &comp)
	{
		comp._isDirty = true;
		_dirtyComponents.add(comp.Handle);
	}

	RenderingComponent::handle_t RenderingSystem::createNew()
	{
		RenderingComponent::handle_t handle = _components.create(*this, _memory);

		return handle;
	}

	void RenderingSystem::remove(RenderingComponent::handle_t handle)
	{
		// Mesh is in some VBO
		// that VBO needs to be destroyed
		// and then recreated, to avoid holes in it

		auto& cmp = _components.get(handle);

	//	_dirtyBatches.add(cmp.getBatchIndex());
		_components.remove(handle);
	}

	RenderingComponent& RenderingSystem::get(RenderingComponent::handle_t handle) const
	{
		return _components.get(handle);
	}

	void RenderingSystem::refreshDirtyComponents()
	{
		for(RenderingComponent::handle_t handle : _dirtyComponents)
		{
			RenderingComponent& comp = _components.get(handle);
			if(comp._batchHandles.size() > 0)
			{
				for (auto& h : comp._batchHandles)
				{
					_engine.BatchManager.get().invalidateBatch(h.BatchHandle);
				}

				comp._batchHandles.clear();
			}

			Core::Mesh& mesh = _engine.MeshManager.get().getMesh(comp.getMesh());
			for (auto matHandle : comp.getMaterials())
			{
				const Core::Material& material = _engine.MaterialManager.get().getMaterial(matHandle);
				comp._batchHandles.add(_engine.BatchManager.get().allocateMesh(mesh, material));
			}

			comp._isDirty = false;
		}

		_dirtyComponents.clear();
	}

	void RenderingSystem::update(const Core::GameTime& time, Gfx::Renderer& renderer)
	{
		refreshDirtyComponents();

		// CULLING WOULD BE DONE HERE?
		// construct Command data for queue
		// pass this queue to renderer

		// Created by renderer? User probably
		// So there should be something like pass or technique class
		// That will get rendering components (or other ones) by type/tag
		// From appropriate system (maybe on separate threads even)!
		// Then it will sort them in renderer
		// And submit calls to GPU
		Gfx::Renderer::queue_t& queue = renderer.getQueue();
		for(RenderingComponent& comp : _components)
		{
			// Get new packet
			// Set key information (this is only used for sorting, co no stress!)
			queue.createCommands(comp);
			// Set Data, like shader program, uniforms, vao etc.
		}

		// submit packet

		// submit to renderer

		// recreate dirty batches
		// add new meshes to them or change their data

	/*	const std::size_t dirtyBatchesCount = _dirtyBatches.size();
		for(std::size_t i = 0; i < dirtyBatchesCount; i++)
		{
			//renderer.recreateBatch(_dirtyBatches[i]);
		}

		const std::size_t dirtyCompCount = _dirtyComponents.size();
		for(std::size_t i = 0; i < dirtyCompCount; i++)
		{
		//	renderer.updateComponent(_dirtyComponents[i]);
		}*/
	}

	/*void RenderingSystem::updateMeshInBucket(Gfx::Renderer& renderer, RenderingComponent& comp)
	{
		Core::Mesh& mesh = comp.getMesh();
		Gfx::RenderPass* bucket = renderer.getBucket(mesh.getMeshData());
		if(bucket != nullptr)
		{
			comp.getBucketIndex()
		}
		else
		{
			// TODO: Log Error!
		}
	}

	void RenderingSystem::whatever()
	{
		for(std::uint32_t i = 0; i < _components.size(); i++)
		{
			Core::Mesh& mesh = _components[i].getMesh();
			auto dataScheme = mesh.getDataScheme();
			Gfx::RenderPass* bucket = renderer.getBucket(dataScheme);
			if(bucket == nullptr)
			{
				bucket = renderer.createBukcet(dataScheme);
			}

			// internal : create batch
			bucket->addMesh(mesh);
		}
	}*/
}
