//
// Created by MrJaqbq on 2016-02-14.
//

#include "Window.h"

namespace Core
{
    Window::Window() : hWindow(nullptr)
    {
        Create();
    }

    Window::~Window()
    {
        Destroy();
    }

    void Window::Create()
    {
        if(IsAlive())
            return;

        hWindow = glfwCreateWindow(200, 200, "Test", nullptr, nullptr);
    }

    void Window::Destroy()
    {
        if(!IsAlive())
            return;

        glfwDestroyWindow(hWindow);
        hWindow = nullptr;
    }

    bool Window::IsAlive()
    {
        return hWindow != nullptr;
    }
}
