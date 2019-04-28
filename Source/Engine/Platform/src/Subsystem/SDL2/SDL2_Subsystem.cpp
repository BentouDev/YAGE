#include "SDL2_Subsystem.h"
#include <SDL2/SDL.h>

namespace yage::platform::sdl2
{
	SDL2::SDL2(Memory::IMemoryBlock* block, SSubsystemParams& params)
		: ISubsystem(block, params)
	{

	}

	WindowHandle SDL2::createWindow(const SWindowParams& params)
	{
		return nullptr;
	}

	bool SDL2::initialize()
	{
		return false;
	}

	void SDL2::pollEvents(Core::EventQueue* queue)
	{

	}

	bool SDL2::destroyWindow(WindowHandle handle)
	{
		return false;
	}

	void SDL2::resizeWindow(WindowHandle handle, unsigned width, unsigned height)
	{

	}

	void SDL2::showWindow(WindowHandle handle)
	{

	}

	void SDL2::makeCurrent(WindowHandle handle)
	{

	}

	void SDL2::endDraw(WindowHandle handle)
	{

	}
}