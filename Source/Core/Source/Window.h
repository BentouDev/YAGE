//
// Created by MrJaqbq on 2016-02-14.
//

#ifndef VOLKHVY_WINDOW_H
#define VOLKHVY_WINDOW_H

#include <string>
#include <vector>
#include "Platform.h"
#include "Viewport.h"

namespace Core
{
    class Window
    {
    public:
        Window();
        virtual ~Window();

        void		Create();
        void		Destroy();
        bool		IsAlive();

        std::string Title;
        unsigned	Width;
        unsigned	Height;

        std::vector<Viewport> Viewports;

        GLFWwindow* hWindow;
    };
}

#endif //VOLKHVY_WINDOW_H
