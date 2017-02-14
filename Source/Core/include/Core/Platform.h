//
// Created by MrJaqbq on 2016-02-14.
//

#ifndef YAGE_PLATFORM_H
#define YAGE_PLATFORM_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <SDL2/SDL.h>

#ifdef CREATE_NEW
#undef CREATE_NEW
#endif

#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef GetCurrentTime
#undef GetCurrentTime
#endif

#endif //YAGE_PLATFORM_H
