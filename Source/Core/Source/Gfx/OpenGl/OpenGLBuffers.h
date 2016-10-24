//
// Created by bentoo on 10/12/16.
//

#ifndef GAME_VAO_H
#define GAME_VAO_H

#include "OpenGLBase.h"

namespace Memory
{
	class IMemoryBlock;
}

namespace OpenGL
{
	class VAO
	{
	private:
		GLuint handle;

		VAO()
		{
			gl::GenVertexArrays(1, &handle);
		}

	public:
		static VAO* Create(Memory::IMemoryBlock& memory);

		~VAO()
		{
			gl::DeleteVertexArrays(1, &handle);
		}

		inline operator GLuint() const
		{
			return handle;
		}

		inline GLuint getHandle() const
		{
			return handle;
		}
	};

	class VBO
	{
	private:
		GLuint handle;

		VBO()
		{
			gl::GenBuffers(1, &handle);
		}

	public:
		static VBO* Create(Memory::IMemoryBlock& memory);

		~VBO()
		{
			gl::DeleteBuffers(1, &handle);
		}

		inline operator GLuint() const
		{
			return handle;
		}

		inline GLuint getHandle() const
		{
			return handle;
		}
	};
}

#endif //GAME_VAO_H
