#include <Platform/Subsystem/ISubsystem.h>
#include <Platform/Subsystem/SubsystemDefinition.h>
#include "Platform/Logger.h"

#include <Utils/MacroDefines.h>
#include <EASTL/hash_map.h>

#include <SDL2/SDL.h>

namespace yage::platform::sdl2
{
    class SDL2 : public ISubsystem
    {
        struct SWindow;

        eastl::hash_map<WindowHandle, SWindow*> _handleLookup;

    public:
        SDL2(Memory::IMemoryBlock* block, SSubsystemParams& params);

        // General
        virtual bool initialize() override;
        virtual void shutdown() override;

        // Devices
        virtual void pollEvents(Core::EventQueue* queue) override;
        virtual double getCurrentTime() override;
        virtual const char* getDeviceName(DeviceHandle handle) override;
        virtual bool isDevicePresent(DeviceHandle handle) override;

        // Windowing
        virtual WindowHandle createWindow(const SWindowParams& params) override;
        virtual bool destroyWindow(WindowHandle handle) override;

        virtual void resizeWindow(WindowHandle handle, unsigned width, unsigned height) override;
        virtual void showWindow(WindowHandle handle) override;
        virtual void makeCurrent(WindowHandle handle) override;
        virtual void endDraw(WindowHandle handle) override;
    };
}

using sdl2_sys = ::yage::platform::sdl2::SDL2;
PUBLISH_SUBSYSTEM(sdl2_sys);

namespace yage::platform::sdl2
{
	SDL2::SDL2(Memory::IMemoryBlock* block, SSubsystemParams& params)
		: ISubsystem(block, params)
	{

	}

	bool SDL2::initialize()
	{
		int errorCode = SDL_Init(SDL_INIT_VIDEO);
		if (errorCode < 0)
		{
			Core::Logger::error("SDL2 : unable to initialize!");
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);// Config::mZBufferBits);

		SDL_ShowCursor(true ? 1 : 0);

		SDL_GL_SetSwapInterval(1);

		return true;
	}

	void SDL2::shutdown()
	{
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		SDL_Quit();
	}

	struct SDL2::SWindow
	{
		SDL_Window* handle;
		SDL_GLContext ctx;
	};

	WindowHandle SDL2::createWindow(const SWindowParams& params)
	{
		SDL_Window* handle = SDL_CreateWindow(params.name.c_str(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			params.width,
			params.height,
            SDL_WINDOW_OPENGL
            | (!params.hidden ? SDL_WINDOW_SHOWN : 0)
            | (params.fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
            | (params.borderless ? SDL_WINDOW_BORDERLESS : 0)
            | (params.resizeable ? SDL_WINDOW_RESIZABLE : 0));

		if (!handle)
		{
			Core::Logger::critical("SDL2 : Unable to create window! Error: '{}'", SDL_GetError());
			return false;
		}

		SDL_GLContext ctx = SDL_GL_CreateContext(handle);

		if (!ctx)
		{
			Core::Logger::critical("SDL2 : Unable to OpenGL context! Error: '{}'", SDL_GetError());
			SDL_DestroyWindow(handle);
			return false;
		}

		SDL2::SWindow* lookup = YAGE_CREATE_NEW((*owningBlock()), SDL2::SWindow)();
		lookup->handle = handle;
		lookup->ctx = ctx;

		_handleLookup[handle] = lookup;

		return handle;
	}

	bool SDL2::destroyWindow(WindowHandle handle)
	{
		auto itr = _handleLookup.find(handle);
		if (itr == _handleLookup.end())
		{
			Core::Logger::error("SDL2 : Unable to destroy window {}, unknown handle!", handle);
			return false;
		}

		auto* window = (*itr).second;

		SDL_GL_DeleteContext(window->ctx);
		SDL_DestroyWindow(window->handle);

		_handleLookup.erase(itr);

		return false;
	}

	void SDL2::pollEvents(Core::EventQueue* queue)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{

		}
	}

	void SDL2::resizeWindow(WindowHandle handle, unsigned width, unsigned height)
	{
		SDL_SetWindowSize(reinterpret_cast<SDL_Window*>(handle), width, height);
	}

	void SDL2::showWindow(WindowHandle handle)
	{
		SDL_ShowWindow(reinterpret_cast<SDL_Window*>(handle));
	}

	void SDL2::makeCurrent(WindowHandle handle)
	{
		auto itr = _handleLookup.find(handle);
		if (itr != _handleLookup.end())
		{
			auto* window = (*itr).second;
			SDL_GL_MakeCurrent(window->handle, window->ctx);
		}
		else
		{
			Core::Logger::error("SDL2 : Unable to make window {} as current, unknown handle!", handle);
		}
	}

	void SDL2::endDraw(WindowHandle handle)
	{
		SDL_GL_SwapWindow(reinterpret_cast<SDL_Window*>(handle));
	}
	
	double SDL2::getCurrentTime()
	{
		return SDL_GetTicks();
	}
	
	const char* SDL2::getDeviceName(DeviceHandle handle)
	{
		return nullptr;
	}
	
	bool SDL2::isDevicePresent(DeviceHandle handle)
	{
		return false;
	}
}