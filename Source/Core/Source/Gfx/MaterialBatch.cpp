//
// Created by bentoo on 11/2/16.
//

#include "MaterialBatch.h"
#include "StaticBuffer.h"
#include "../Resources/Material/Material.h"
#include "../Resources/Mesh/Mesh.h"

namespace Gfx
{
	bool MaterialBatch::hasMaterial(const Core::Material &material) const
	{
		return hasMaterial(material.Handle);
	}

	bool MaterialBatch::hasMaterial(Utils::Handle<Core::Material> material) const
	{
		return material == _material;
	}

	/*bool MaterialBatch::hasSize(uint32_t vertexCount, uint32_t indexCount) const
	{
		for(StaticBuffer& buffer : _staticBuffers)
		{
			if(buffer.hasSize(vertexCount, indexCount))
			{
				return true;
			}
		}

		return false;
	}*/

	MaterialBatchMeshReference MaterialBatch::allocateMesh(const Core::Mesh &mesh)
	{
		return MaterialBatchMeshReference();
	/*	if(batch.hasSize(mesh.vertexCount(), submeshes[i]._indiceCount))
		{
			batch.allocate(mesh, i);
			break;
		}
		else
		{
			// Create new buffer for this batch to allocate mesh
		}*/
	}
}