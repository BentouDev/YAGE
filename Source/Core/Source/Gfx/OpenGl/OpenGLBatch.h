//
// Created by bentoo on 10/12/16.
//

#ifndef GAME_OPENGLBATCH_H
#define GAME_OPENGLBATCH_H

#include <cstdio>
#include <Resources/Mesh/Mesh.h>
#include "OpenGLBuffers.h"

namespace OpenGL
{
	class Batch
	{
		Core::MeshData scheme;

		VAO vao;
		GenericBuffer vbo;
	//	IBO ibo;
		std::size_t vboMaxSize;

		struct MeshInfo
		{
			Core::Mesh::handle_t meshHandle;
			uint32_t baseVertex;
		};

		Utils::List<MeshInfo> meshesToDraw;
	};
}

#endif //GAME_OPENGLBATCH_H
