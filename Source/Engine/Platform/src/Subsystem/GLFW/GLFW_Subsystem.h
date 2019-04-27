#ifndef YAGE_GLFW_WINDOW_H
#define YAGE_GLFW_WINDOW_H

#include <Platform/Subsystem/ISubsystem.h>
#include <Subsystem/SubsystemDefinition.h>
#include <EASTL/hash_map.h>

struct GLFWwindow;

namespace Core
{
	class EventQueue;
}

namespace yage::platform::glfw
{
	class GLFW : public ::yage::platform::ISubsystem
	{
		void registerWindowCallbacks(GLFWwindow* window);
		void registerGlobalCallbacks();

	public:
		GLFW(Memory::IMemoryBlock* block, SSubsystemParams& params);

		Core::EventQueue* getQueue();

		// General
		virtual bool initialize() override;
		virtual void shutdown() override;
		virtual void pollEvents(Core::EventQueue* queue) override;
		virtual double getCurrentTime() override;

		// Devices
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

using glfw_sys = yage::platform::glfw::GLFW;
PUBLISH_SUBSYSTEM(glfw_sys);

#endif YAGE_GLFW_WINDOW_H