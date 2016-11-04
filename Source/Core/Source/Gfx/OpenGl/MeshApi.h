//
// Created by bentoo on 10/12/16.
//

#ifndef GAME_MESHAPI_H
#define GAME_MESHAPI_H

#include <Utils/List.h>
#include "OpenGLBuffers.h"

namespace Gfx
{
	class MeshApi
	{
		Utils::List<OpenGL::GenericBuffer> _vbos;
		OpenGL::VAO* _vao;
	};
}

#endif //GAME_MESHAPI_H
