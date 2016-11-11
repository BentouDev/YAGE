//
// Created by Bentoo on 2016-09-22.
//

#include "OpenGLBase.h"

#include <Window.h>
#include "../../Platform.h"
#include "../../Logger.h"

namespace OpenGL
{
    gl::exts::LoadTest didLoadFunctions;

	auto initialize() -> bool
	{
		if(!glfwInit())
			return false;

		// todo: pick from config
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, gl::TRUE_);

		return true;
	}

	void beginDraw(const Core::Window& window)
	{
		glfwMakeContextCurrent(window.hWindow);
		gl::ClearColor(0.5f, 0.75f, 0.25f, 0.0f);
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
	}

	void endDraw(const Core::Window& window)
	{
		glfwSwapBuffers(window.hWindow);
	}

    bool registerWindow(const Core::Window& window)
    {
		glfwMakeContextCurrent(window.hWindow);
		if(!didLoadFunctions)
		{
			didLoadFunctions = gl::sys::LoadFunctions();
		}

		gl::Enable(gl::DEPTH_TEST);

        return didLoadFunctions;
    }

	void resizeWindow(const Core::Window& window)
	{
		gl::Viewport(0, 0, window.Width, window.Height);
	}

	bool checkError(Core::Context context)
	{
		GLenum errorCode = gl::GetError();
		if(errorCode != gl::NO_ERROR_)
		{
			switch(errorCode)
			{
				case gl::INVALID_ENUM:
					context.Logger->Default->error("An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.");
					break;

				case gl::INVALID_VALUE:
					context.Logger->Default->error("A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.");
					break;

				case gl::INVALID_OPERATION:
					context.Logger->Default->error("The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.");
					break;

				case gl::INVALID_FRAMEBUFFER_OPERATION:
					context.Logger->Default->error("The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.");
					break;

				case gl::OUT_OF_MEMORY:
					context.Logger->Default->error("There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.");
					break;

				/*case gl::STACK_UNDERFLOW:
					context.Logger->Default->error("An attempt has been made to perform an operation that would cause an internal stack to underflow.");

				case gl::STACK_OVERFLOW:
					context.Logger->Default->error("An attempt has been made to perform an operation that would cause an internal stack to overflow.");*/

				default:
					context.Logger->Default->critical("Unknown OpenGL, possibly memory corruption!");
			}

			return true;
		}

		return false;
	}

	GLenum toOpenGlType(type_t type)
	{
		static std::map<type_t, GLenum> dispatcher = {
			{ TypeInfo<float>::id(), gl::FLOAT },
			{ TypeInfo<double>::id(), gl::DOUBLE },
			{ TypeInfo<signed char>::id(), gl::BYTE },
			{ TypeInfo<unsigned char>::id(), gl::UNSIGNED_BYTE },
			{ TypeInfo<int16_t>::id(), gl::SHORT },
			{ TypeInfo<uint16_t>::id(), gl::UNSIGNED_SHORT },
			{ TypeInfo<int32_t>::id(), gl::INT },
			{ TypeInfo<uint32_t>::id(), gl::UNSIGNED_INT },
		};

		GLenum result = 0;

		auto itr = dispatcher.find(type);
		if(itr != dispatcher.end())
		{
			return itr->second;
		}

		else return gl::BYTE;

		/*switch(type)
		{
			case TypeInfo<float>::id():
				result = gl::FLOAT;
				break;
			case TypeInfo<double>::id():
				result = gl::DOUBLE;
				break;
			case TypeInfo<signed char>::id():
				result = gl::BYTE;
				break;
			case TypeInfo<unsigned char>::id():
				gl::UNSIGNED_BYTE;
				break;
			case TypeInfo<int16_t>::id():
				result = gl::SHORT;
				break;
			case TypeInfo<uint16_t>::id():
				result = gl::UNSIGNED_SHORT;
				break;
			case TypeInfo<int32_t >::id():
				result = gl::INT;
				break;
			case TypeInfo<uint32_t >::id():
				result = gl::UNSIGNED_INT;
				break;
			default:
				result = gl::BYTE;
		}

		return result;*/
	}
}