//
// Created by bentoo on 11/3/16.
//

#include "Core/Gfx/StaticBuffer.h"

namespace Gfx
{
	void* StaticBuffer::mapVertexMemory(std::size_t vertexCount, GLenum flags)
	{
		gl::BindBuffer(gl::ARRAY_BUFFER, getVBO());
		return gl::MapBufferRange(gl::ARRAY_BUFFER, _usedVertices * _vertexSize, vertexCount * _vertexSize, flags);
	}

	void* StaticBuffer::mapIndexMemory(std::size_t indexCount, GLenum flags)
	{
		gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, (*_ibo));
		return gl::MapBufferRange(gl::ELEMENT_ARRAY_BUFFER, _usedIndices * _indexSize, indexCount * _indexSize, flags);
	}

	void StaticBuffer::unmapVertexMemory()
	{
		gl::BindBuffer(gl::ARRAY_BUFFER, getVBO());
		gl::UnmapBuffer(gl::ARRAY_BUFFER);
	}

	void StaticBuffer::unmapIndexMemory()
	{
		gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, getIBO());
		gl::UnmapBuffer(gl::ELEMENT_ARRAY_BUFFER);
	}

	void StaticBuffer::clear()
	{
		_usedIndices = 0;
		_usedVertices = 0;
	}
}
