//
// Created by bentoo on 10/19/16.
//

#include "Renderer.h"
#include "../Engine.h"
#include "../Resources/Mesh/Mesh.h"
#include "../Logic/RenderingSystem.h"

namespace Gfx
{
	Renderer::Renderer(Core::Engine& engine, Memory::IMemoryBlock& memory)
		: _engine(engine), _memory(memory), _buckets(_memory), _context(_engine.GetContext())
	{ };

//	void Renderer::recreateBatch(uint32_t key)
//	{

//	}

	void Renderer::updateComponent(Logic::RenderingComponent &comp)
	{
		/*RenderBatch& bucket = getBucket(comp.getMesh().getMeshData());

		if(comp.getBatchIndex() < 0)
		{
			comp.setBatchIndex(bucket.addMesh(comp.getMesh()));
		}
		else
		{
			bucket.updateMesh(comp.getBatchIndex(), comp.getMesh());
		}*/
	}

	void Renderer::draw()
	{
		const std::size_t bucketCount = _buckets.size();
		for(std::size_t i = 0; i < bucketCount; i++)
		{
			const Gfx::RenderPass& bucket = _buckets[i];
			const Utils::List<Gfx::RenderBatch>& batches = bucket.getBatches();
			const std::size_t batchCount = batches.size();

			//

			// do vbo upload data by scheme, as intended


			for(std::size_t j = 0; j < batchCount; j++)
			{
				const Gfx::RenderBatch& batch = batches[j];
				const Utils::List<Gfx::RenderBatch::MaterialInfo>& materials = batch.getMaterialBatches();
				const std::size_t materialCount = materials.size();

				// batch.getVAO().bind();
				// which binds vbo and ibo with apropriate strides

				for (int k = 0; k < materialCount; k++)
				{
					const Gfx::RenderBatch::MaterialInfo& material = materials[k];
					const std::size_t submeshCount = material.submeshes.size();

					// binds material data for all meshes as a buffer
					// bind shader ofc
					// but bind vao?

					for(std::size_t l = 0; l < submeshCount; l++)
					{
						const Gfx::RenderBatch::SubmeshInfo& mesh = material.submeshes[l];
						gl::DrawElementsBaseVertex(gl::TRIANGLES, mesh.count, mesh.indexType, 0, mesh.baseVertex);
					}
				}
			}
		}
	}
}