//
// Created by bentoo on 10/19/16.
//

#include "Renderer.h"
#include "BatchManager.h"
#include "../Engine.h"
#include "../Resources/Mesh/Mesh.h"
#include "../Resources/Mesh/MeshManager.h"
#include "../Logic/RenderingSystem.h"
#include <Utils/Handle.h>

namespace Gfx
{
	Renderer::Renderer(Core::Engine& engine, Memory::IMemoryBlock& memory)
		: _engine(engine), _memory(memory), _buckets(_memory), _context(_engine.GetContext()),
		  lastProgram(0), lastVAO(0), _queue(_memory, *this)
			/*_queue
			(
				_memory,
				[&](RenderData& data){this->drawCall(data);},
				[&](Logic::RenderingComponent& component){this->createDrawCallData(component);}
			),*/
	{

	};

	void Renderer::drawCall(RenderData& data)
	{
		if(lastProgram != data.ShaderProgram)
		{
			lastProgram = data.ShaderProgram;
			gl::UseProgram(data.ShaderProgram);
		}

		if(lastVAO != data.VAO)
		{
			lastVAO = data.VAO;
			gl::BindVertexArray(data.VAO);
		}

		gl::DrawElementsBaseVertex(gl::TRIANGLES, data.elementCount, data.indexType, 0, data.baseVertex);
	}

	void Renderer::createDrawCallData(Logic::RenderingComponent& component)
	{
		for(MaterialBatchMeshReference& reference : component._batchHandles)
		{
			RenderKey&		key	 = _queue._keys.emplace();
			RenderData&		data = _queue._data.emplace();

			// set values
			data.ShaderProgram	= reference.ShaderProgram;
			data.baseVertex		= reference.BaseVertex;
			data.elementCount	= reference.ElementCount;
			data.indexType		= reference.IndexType;
			data.VAO			= (*reference.VAO);
		}

		return;

		// key doesnt matter for now
		// but we can have submeshes, they should be bundled together
		// So component really has reference to our batch
		// And that bath is per material per vertex format
		// So we get those bathes using component indices
		// And create commands from them here
		// Should have multiple outputs here tho

		// Probably this mesh lookup is unnecessary here
		const Core::Mesh& mesh = _engine.MeshManager.get().getMesh(component.getMesh());
		const Utils::List<Core::Submesh>& submeshes = mesh.getSubmeshes();

		for(std::size_t i = 0; i < submeshes.size(); i++)
		{
			submeshes[i];
			component.getMaterial(i);
		}
	}

	void Renderer::draw()
	{
		_queue.sort();
		_queue.issueCommands();

		/*const std::size_t bucketCount = _buckets.size();
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
		}*/
	}
}