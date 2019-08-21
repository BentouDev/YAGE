//
// Created by bentoo on 10/14/16.
//

#include "Gfx/Graphics/OpenGl/OpenGLBuffers.h"
#include <Utils/DebugSourceInfo.h>
#include <Utils/MemoryBlock.h>

namespace OpenGL
{
	VAO* VAO::Create(Memory::IMemoryBlock& memory)
	{
		return YAGE_CREATE_NEW(memory, VAO)();
	}

	GenericBuffer* GenericBuffer::Create(Memory::IMemoryBlock& memory)
	{
		return YAGE_CREATE_NEW(memory, VBO)();
	}
}