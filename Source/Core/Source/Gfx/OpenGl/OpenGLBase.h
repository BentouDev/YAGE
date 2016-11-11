//
// Created by Bentoo on 2016-09-22.
//

#ifndef GAME_OPENGLBASE_H
#define GAME_OPENGLBASE_H

#include "Generated/gl_core_3_3.hpp"
#include <Utils/TypeInfo.h>

#ifdef CREATE_NEW
#undef CREATE_NEW
#endif

#ifdef CreateWindow
#undef CreateWindow
#endif

namespace Core
{
	class Window;
	class Context;
}

namespace OpenGL
{
	extern gl::exts::LoadTest didLoadFunctions;

	auto initialize() -> bool;

	void beginDraw(const Core::Window& window);

	void endDraw(const Core::Window& window);

	void resizeWindow(const Core::Window& window);

	bool registerWindow(const Core::Window& window);

	bool checkError(Core::Context context);

	GLenum toOpenGlType(type_t type);
}

#endif //GAME_OPENGLBASE_H
