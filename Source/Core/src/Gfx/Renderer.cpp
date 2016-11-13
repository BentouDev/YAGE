//
// Created by bentoo on 10/19/16.
//

#include <glm/gtc/matrix_transform.hpp>

#include "Core/Gfx/Renderer.h"

#include "Core/Engine.h"
#include "Core/Resources/Mesh/Mesh.h"
#include "Core/Resources/Mesh/MeshManager.h"
#include "Core/Logic/RenderingSystem.h"

namespace Gfx
{
	Renderer::Renderer(Core::Engine& engine, Memory::IMemoryBlock& memory)
		: _engine(engine), _context(_engine.GetContext()),_memory(memory), _queue(_memory, *this),
		  lastIBO(0), lastVBO(0), lastVAO(0), lastProgram(0)
	{

	}

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

		if(lastVBO != data.VBO)
		{
			lastVBO = data.VBO;
			gl::BindBuffer(gl::ARRAY_BUFFER, data.VBO);
		}

		if(lastIBO != data.IBO)
		{
			lastIBO = data.IBO;
			gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, data.IBO);
		}

		gl::DrawElementsBaseVertex(gl::TRIANGLES, data.elementCount, OpenGL::toOpenGlType(data.indexType), 0, data.baseVertex);
	}

	void Renderer::draw()
	{
		glm::mat4 Projection = glm::perspective(
			3.14f / 4.0f,         // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
			4.0f / 3.0f, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
			0.01f,        // Near clipping plane. Keep as big as possible, or you'll get precision issues.
			100.0f       // Far clipping plane. Keep as little as possible.
		);

		glm::mat4 View = glm::lookAt(
				glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
				glm::vec3(0,0,0), // and looks at the origin
				glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model = glm::mat4(1.0f);

		gl::UniformMatrix4fv(1, 1, gl::FALSE_, &Projection[0][0]);
		gl::UniformMatrix4fv(2, 1, gl::FALSE_, &View[0][0]);
		gl::UniformMatrix4fv(3, 1, gl::FALSE_, &Model[0][0]);

		//_queue.sort();
		//_queue.issueCommands();

		for(auto& data : _queue._data)
		{
			drawCall(data);
		}

		_queue._data.clear();
		_queue._keys.clear();

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