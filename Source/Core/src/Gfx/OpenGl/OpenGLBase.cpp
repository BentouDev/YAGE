//
// Created by Bentoo on 2016-09-22.
//

#include "Core/Gfx/OpenGl/OpenGLBase.h"

#include "Core/Window.h"
#include "Core/Platform.h"
#include "Core/Logger.h"

namespace OpenGL
{
    gl::exts::LoadTest didLoadFunctions;

	auto initialize() -> bool
	{
		auto code = SDL_Init(SDL_INIT_VIDEO);
		if(code != 0)
		{
			Core::Logger::error("SDL : unable to initialize video system : code '{}'\n\t'{}'", code, SDL_GetError());
			return false;
		}

		// todo: pick from config

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, SDL_TRUE);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); /*Config::mZBufferBits*/

		SDL_GL_SetSwapInterval(1);

		return true;
	}

	void beginDraw(const Core::Window& window)
	{
		SDL_GL_MakeCurrent(window.hWindow, window.hContext);
		gl::ClearColor(0.5f, 0.75f, 0.25f, 0.0f);
		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
	}

	void endDraw(const Core::Window& window)
	{
		SDL_GL_SwapWindow(window.hWindow);
	}

    bool registerWindow(const Core::Window& window)
    {
		SDL_GL_MakeCurrent(window.hWindow, window.hContext);
		if(!didLoadFunctions)
		{
			didLoadFunctions = gl::sys::LoadFunctions();
		}

		gl::Enable(gl::DEPTH_TEST);
		// gl::Enable(gl::TEXTURE_2D);

		OpenGL::checkError();

		return didLoadFunctions;
	}

	void resizeWindow(const Core::Window& window)
	{
		gl::Viewport(0, 0, window.Width, window.Height);
	}

	bool checkError()
	{
		GLenum errorCode = gl::GetError();
		if(errorCode != gl::NO_ERROR_)
		{
			switch(errorCode)
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

		auto itr = dispatcher.find(type);
		if(itr != dispatcher.end())
		{
			return itr->second;
		}

		else return gl::BYTE;
	}
}