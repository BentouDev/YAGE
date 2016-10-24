//
// Created by bentoo on 10/14/16.
//

#include "OpenGLBuffers.h"
#include <Utils/DebugSourceInfo.h>
#include <Utils/MemoryBlock.h>

namespace OpenGL
{
	VAO* VAO::Create(Memory::IMemoryBlock& memory)
	{
		return YAGE_CREATE_NEW(memory, VAO)();
	}

	VBO* VBO::Create(Memory::IMemoryBlock& memory)
	{
		return YAGE_CREATE_NEW(memory, VBO)();
	}
}