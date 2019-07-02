#ifndef YAGE_SDL2_SUBSYSTEM_H
#define YAGE_SDL2_SUBSYSTEM_H

#include <Platform/Subsystem/ISubsystem.h>
#include <Platform/Subsystem/SubsystemDefinition.h>
#include <Utils/MacroDefines.h>
#include <EASTL/hash_map.h>

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

#endif//YAGE_SDL2_SUBSYSTEM_H