//
// Created by bentoo on 10/14/16.
//

#include <cstdint>
#include <Utils/Container.h>
#include "../Gfx/RenderBatch.h"
#include "RenderingSystem.h"
#include "../Resources/Mesh/Mesh.h"

namespace Logic
{
	void RenderingComponent::setMaterial(Core::Material& material, std::uint32_t index)
	{
		if(index < _debugMesh->getSubmeshes().size())
		{
			_debugMaterials.resize(index + 1);
			_debugMaterials[index] = &material;
		}
	}

	RenderingSystem::RenderingSystem(Core::Engine &engine, Memory::IMemoryBlock& memory)
		: _engine(engine), _memory(memory), _components(memory)//, _dirtyComponents(memory), _dirtyBatches(memory)
	{

	}

	RenderingComponent::handle_t RenderingSystem::createNew()
	{
		RenderingComponent::handle_t result = _components.create(_memory);
		RenderingComponent& component = _components.get(result);

	//	_dirtyComponents.add(&component);

		return result;
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

	void RenderingSystem::update(const Core::GameTime& time, Gfx::Renderer& renderer)
	{
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
