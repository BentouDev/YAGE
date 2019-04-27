//
// Created by Bentoo on 2016-09-22.
//

#ifndef GAME_OPENGLBASE_H
#define GAME_OPENGLBASE_H

#include "Platform/Graphics/OpenGL/gl_core_3_3.hpp"
#include <RTTI/Defines.h>

#ifdef CREATE_NEW
#undef CREATE_NEW
#endif

#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef GetCurrentTime
#undef GetCurrentTime
#endif

namespace Core
{
    class Window;
    class Logger;
}

namespace OpenGL
{
    extern gl::exts::LoadTest didLoadFunctions;

    void beginDraw(const Core::Window& window);

    void endDraw(const Core::Window& window);

    void resizeWindow(const Core::Window& window);

    bool registerWindow(const Core::Window& window);

    bool checkError();

    GLenum toOpenGlType(type_t type);
}

#endif //GAME_OPENGLBASE_H
