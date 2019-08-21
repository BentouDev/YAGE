//
// Created by Bentoo on 2016-09-26.
//

#ifndef GAME_ENGINEAPIS_H
#define GAME_ENGINEAPIS_H

//#include "Gfx/Api/BaseApi.h"

#ifdef YAGE_VULKAN
// #include "Gfx/Vulkan/VulkanDevice.h"
#endif

#ifdef YAGE_OPENGL
// #include "Gfx/OpenGl/OpenGlContext.h"
#include "Gfx/Graphics//OpenGl/OpenGLBase.h"
#endif

#include "Platform/Platform.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

#endif //GAME_ENGINEAPIS_H
