//
// Created by Bentoo on 2016-09-22.
//

#include "Platform/Graphics/OpenGl/OpenGLBase.h"

#include "Platform/Subsystem/ISubsystem.h"
#include "Platform/Window.h"
#include "Platform/Platform.h"
#include "Platform/Logger.h"

#include <EASTL/hash_map.h>

namespace OpenGL
{
    gl::exts::LoadTest didLoadFunctions;

    void beginDraw(const Core::Window& window)
    {
        if (!window.IsAlive())
            return;

		yage::platform::getSubsystem().makeCurrent(window.GetNative());

		gl::ClearColor(0.5f, 0.75f, 0.25f, 0.0f);
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
    }

    void endDraw(const Core::Window& window)
    {
        if(!window.IsAlive())
            return;

		yage::platform::getSubsystem().endDraw(window.GetNative());
    }

    bool registerWindow(const Core::Window& window)
    {
        if (!window.IsAlive())
            return false;

		yage::platform::getSubsystem().makeCurrent(window.GetNative());
        if (!didLoadFunctions)
        {
            didLoadFunctions = gl::sys::LoadFunctions();
        }

        return (bool) didLoadFunctions;
    }

    void resizeWindow(const Core::Window& window)
    {
        if (!window.IsAlive())
            return;

		yage::platform::getSubsystem().makeCurrent(window.GetNative());
        // for(auto& view : window.getViewports())
        auto& view = window.GetDefaultViewport();
        {
            const auto& rect = view.getPixelRect();
            gl::Viewport(rect.getLeft(), rect.getBottom(), rect.getWidth(), rect.getHeight());
        }
    }

    bool checkError()
    {
        GLenum errorCode = gl::GetError();
        if (errorCode != gl::NO_ERROR_)
        {
            switch (errorCode)
            {
                case gl::INVALID_ENUM:
                    Core::Logger::error("An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.");
                    break;

                case gl::INVALID_VALUE:
                    Core::Logger::error("A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.");
                    break;

                case gl::INVALID_OPERATION:
                    Core::Logger::error("The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.");
                    break;

                case gl::INVALID_FRAMEBUFFER_OPERATION:
                    Core::Logger::error("The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.");
                    break;

                case gl::OUT_OF_MEMORY:
                    Core::Logger::error("There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.");
                    break;

                /*case gl::STACK_UNDERFLOW:
                    Logger::get()->error("An attempt has been made to perform an operation that would cause an internal stack to underflow.");

                case gl::STACK_OVERFLOW:
                    Logger::get()->error("An attempt has been made to perform an operation that would cause an internal stack to overflow.");*/

                default:
                    Core::Logger::critical("Unknown OpenGL, possibly memory corruption!");
            }

            return true;
        }

        return false;
    }

    GLenum toOpenGlType(type_t type)
    {
        // ToDo : Fix OpenGL type dispatching
        static eastl::hash_map<type_t, GLenum> dispatcher;/* = {
            { TypeInfo<float>::id(), gl::FLOAT },
            { TypeInfo<double>::id(), gl::DOUBLE },
            { TypeInfo<signed char>::id(), gl::BYTE },
            { TypeInfo<unsigned char>::id(), gl::UNSIGNED_BYTE },
            { TypeInfo<int16_t>::id(), gl::SHORT },
            { TypeInfo<uint16_t>::id(), gl::UNSIGNED_SHORT },
            { TypeInfo<int32_t>::id(), gl::INT },
            { TypeInfo<uint32_t>::id(), gl::UNSIGNED_INT },
        }; */

        auto itr = dispatcher.find(type);
        if (itr != dispatcher.end())
        {
            return itr->second;
        }

        else return gl::BYTE;
    }
}