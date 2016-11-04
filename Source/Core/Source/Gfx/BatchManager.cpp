//
// Created by bentoo on 10/24/16.
//

#include "BatchManager.h"
#include "StaticBuffer.h"
#include "../Resources/Material/Material.h"
#include "../Resources/Mesh/Mesh.h"

namespace Gfx
{
	BatchManager::BatchManager(Core::Engine& engine, Memory::IMemoryBlock& memory)
		: _engine(engine), _memory(memory), _materialBatchesContainer(_memory), _staticBuffersContainer(_memory)
	{

	}

	BatchManager::~BatchManager()
	{

	}

	MaterialBatch& BatchManager::getBatch(MaterialBatch::handle_t handle)
	{
		return _materialBatchesContainer.get(handle);
	}

	void BatchManager::allocateMesh(MaterialBatch& batch, const Core::Mesh& mesh)
	{

	}

	/*void BatchManager::findStaticBufferInMaterial(MaterialBatch& batch, Resources::MeshSchemeId scheme, std::size_t vertexCount, std::size_t indexCount)
	{
		StaticBuffer* mostFitting = nullptr;
		std::size_t smallestDiff = vertexCount;

		for(StaticBuffer::handle_t handle : batch._staticBuffers)
		{
			StaticBuffer& staticBuffer = _staticBuffersContainer.get(handle);
			if(staticBuffer.hasSize(vertexCount, indexCount))
			{
				std::size_t freeVertices = staticBuffer.getVertexCount() - staticBuffer.getUsedVertexCount();
				if(freeVertices < smallestDiff)
				{
					smallestDiff = freeVertices;
					mostFitting = &staticBuffer;
				}
			}

			if(mostFitting != nullptr)
			{
				// Allocate mesh here
			}
			else
			{
				// create new batch
			}
		}
	}*/

	MaterialBatchMeshReference BatchManager::allocateMesh(const Core::Mesh& mesh, const Core::Material& material)
	{
		MaterialBatchMeshReference result;
		bool allocated = false;

		for(MaterialBatch& batch : _materialBatchesContainer)
		{
			if(batch.hasMaterial(material))
			{
				allocateMesh(batch, mesh);
				allocated = true;
				break;
			}
		}

		if(!allocated)
		{
			// Create new batch for this material
			//	MaterialBatch::handle_t handle = _materialBatchesContainer.create(material);
			//	MaterialBatch& batch = _materialBatchesContainer.get(handle);
			//	batch.allocateMesh(mesh, i);
		}

		return result;
	}

	void BatchManager::allocateMeshes(const Core::Mesh &mesh, Utils::List<Core::Material> &materials, Utils::List<MaterialBatchMeshReference>& result)
	{
		assert(mesh.getSubmeshes().size() == materials.size() && "Count of materials and submeshes cannot differ!");

		for(std::size_t i = 0; i < materials.size(); i++)
		{
			const Core::Material& material = materials[i];
			result.add(allocateMesh(mesh, material));
		}
	}

	// TODO: pass mesh as parameter (so we know what was causing invalidation)
	void BatchManager::invalidateBatch(MaterialBatch::handle_t handle)
	{
		// TODO: remove that mesh from list
		// TODO: recreate batch for meshes that still use it
	}
}
