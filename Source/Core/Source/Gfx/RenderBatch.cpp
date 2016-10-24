//
// Created by bentoo on 10/20/16.
//

#include "RenderBatch.h"
#include "RenderPass.h"

namespace Gfx
{
	RenderBatch::RenderBatch(const RenderPass& parentBucket, const Core::MeshData& scheme) noexcept
		: _parentBucket(parentBucket), _materialBatches(_parentBucket.getMemory()), _scheme(scheme)
	{

	}

	RenderBatch::~RenderBatch()
	{
		cleanUp();
	}

	void RenderBatch::init()
	{
		_vao = OpenGL::VAO::Create(_parentBucket.getMemory());
		_vbo = OpenGL::VBO::Create(_parentBucket.getMemory());
		_ibo = OpenGL::VBO::Create(_parentBucket.getMemory());
	}

	void RenderBatch::cleanUp()
	{
		Memory::Delete(_parentBucket.getMemory(), _vao);
		Memory::Delete(_parentBucket.getMemory(), _vbo);
		Memory::Delete(_parentBucket.getMemory(), _ibo);
	}

	void RenderBatch::addMesh(const Core::Mesh &mesh)
	{

	}

	void RenderBatch::updateMesh(uint32_t index, const Core::Mesh &mesh)
	{

	}
}