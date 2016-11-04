//
// Created by bentoo on 10/27/16.
//

#ifndef GAME_STATICBATCH_H
#define GAME_STATICBATCH_H

#include <cstdint>
#include <Utils/Handle.h>
#include <Utils/MemoryBlock.h>
#include <Utils/DefaultTrait.h>
#include "OpenGl/OpenGLBuffers.h"
#include "../Context.h"

namespace Gfx
{
	struct MeshBufferReference
	{
		GLuint	 		baseVertex;
		OpenGL::VBO* 	vbo;
		OpenGL::IBO*	ibo;
	};

	class StaticBuffer
	{
	public:
		using handle_t = Utils::Handle<StaticBuffer>;

		handle_t Handle;

	protected:
		Memory::IMemoryBlock& _memory;

		OpenGL::VBO* 	_vbo;
		OpenGL::IBO*	_ibo;

		// VERTEX FORMAT
		// SIZE OF ONE VERTEX (DATA IS AOS)

		std::size_t	_vertexSize;
		std::size_t	_indexSize;
		std::size_t	_vertexCount;
		std::size_t	_indexCount;

		std::size_t	_usedVertices;
		std::size_t	_usedIndices;

	public:
		inline explicit StaticBuffer(Memory::IMemoryBlock& memory,
							  std::size_t vertexSize, std::size_t indexSize,
							  std::size_t vertexCount, std::size_t indexCount, Core::Context* context = nullptr)
			: _memory(memory), _vbo(nullptr), _ibo(nullptr),
			  _vertexSize(vertexSize), _indexSize(indexSize),
			  _vertexCount(vertexCount), _indexCount(indexCount),
			  _usedVertices(0), _usedIndices(0)
		{
			_vbo = OpenGL::VBO::Create(_memory);
			_ibo = OpenGL::IBO::Create(_memory);

			gl::BindBuffer(gl::ARRAY_BUFFER, getVBO());
			gl::BufferData(gl::ARRAY_BUFFER, vertexSize * vertexCount, nullptr, gl::STATIC_DRAW);

			if(context != nullptr)
				OpenGL::checkError(*context);

			gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, getIBO());
			gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, indexSize * indexCount, nullptr, gl::STATIC_DRAW);

			if(context != nullptr)
				OpenGL::checkError(*context);

			gl::BindBuffer(gl::ARRAY_BUFFER, 0);
			gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0);
		}

		inline virtual ~StaticBuffer() noexcept
		{
			Memory::Delete(_memory, _vbo);
			Memory::Delete(_memory, _ibo);
		}

		void clear();

		void* mapVertexMemory(std::size_t vertexCount, GLenum flags);
		void* mapIndexMemory(std::size_t indexCount, GLenum flags);

		void unmapVertexMemory();
		void unmapIndexMemory();

		inline bool hasSize(std::size_t vertexCount, std::size_t indexCount) const
		{
			return (_indexCount - _usedIndices) >= indexCount
			    && (_vertexCount - _usedVertices) >= vertexCount;
		}

		inline const OpenGL::VBO& getVBO() const
		{ return *_vbo; }

		inline const OpenGL::IBO& getIBO() const
		{ return *_ibo; }

		inline std::size_t getVertexCount() const
		{ return _vertexCount; }

		inline std::size_t getIndexCount() const
		{ return _indexCount; }

		inline std::size_t getUsedVertexCount() const
		{ return _usedVertices; }

		inline std::size_t getUsedIndexCount() const
		{ return _usedIndices; }

		virtual auto swap(StaticBuffer& other) noexcept -> void
		{ }

		virtual auto cleanUp() noexcept -> void
		{ }
	};

	class StaticBufferTrait : public Utils::DefaultTrait<StaticBuffer> { };
}

#endif //GAME_STATICBATCH_H
