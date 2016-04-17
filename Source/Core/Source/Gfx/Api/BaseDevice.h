//
// Created by MrJaqbq on 2016-03-11.
//

#ifndef VOLKHVY_BASEAPI_H
#define VOLKHVY_BASEAPI_H

namespace Core
{
    class Window;
	class Engine;
}

namespace Gfx
{
	class BaseDevice
	{
		const char* _name;
	public:

		BaseDevice(const char* name) : _name(name) {}

		auto name() -> const char* { return _name; }

		// todo: pass config
		// Do necassary initialization, grab extensions, device info etc.
		virtual auto initialize() -> bool = 0;

		// Create surface for window
		virtual auto registerWindow(const Core::Window& window) -> bool = 0;

		virtual auto resizeWindow(const Core::Window& window) -> void = 0;

		virtual auto destroyWindow(const Core::Window& window) -> bool = 0;

		virtual auto destroyWindows() -> void = 0;

		virtual auto cleanUp() -> void = 0;

		virtual void beginDraw(const Core::Window& window) = 0;

		virtual void endDraw(const Core::Window& window) = 0;

		virtual ~BaseDevice() {}
	};
}

#endif //VOLKHVY_BASEAPI_H
