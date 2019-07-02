#ifndef YAGE_SUBSYSTEM_H
#define YAGE_SUBSYSTEM_H

#include <Utils/MemoryBlock.h>
#include <Utils/Defines.h>
#include <Utils/BorrowedPtr.h>
#include <EASTL/string.h>
#include <Platform/Platform.h>

namespace Core
{
	class EventQueue;
	class Config;
}

namespace yage::platform
{
	struct SSubsystemParams
	{
		Core::EventQueue* eventQueue{ nullptr };
	};

	struct SWindowParams
	{
        eastl::string name;
        unsigned width;
        unsigned height;
        bool hidden : 1;
        bool borderless : 1;
        bool fullscreen : 1;
        bool resizeable : 1;
	};

	class ISubsystem
	{
	protected:
		Memory::IMemoryBlock* _owningBlock;
		Core::EventQueue* _eventQueue;

		// No copying or moving
		ISubsystem(ISubsystem&&) = delete;
		ISubsystem(const ISubsystem&) = delete;
		ISubsystem& operator=(ISubsystem&&) = delete;
		ISubsystem& operator=(const ISubsystem&) = delete;

	public:
		ISubsystem(Memory::IMemoryBlock* block, SSubsystemParams& params)
			: _owningBlock(block), _eventQueue(params.eventQueue)
		{ }
		
		virtual ~ISubsystem()
		{ }

		// General
		virtual bool initialize() = 0;
		virtual void shutdown() = 0;
		virtual void pollEvents(Core::EventQueue* queue) = 0;
		virtual double getCurrentTime() = 0;

		// Devices
		virtual const char* getDeviceName(DeviceHandle handle) = 0;
		virtual bool isDevicePresent(DeviceHandle handle) = 0;

		// Windowing		
		virtual WindowHandle createWindow(const SWindowParams& params) = 0;
		virtual bool destroyWindow(WindowHandle handle) = 0;

		virtual void resizeWindow(WindowHandle handle, unsigned width, unsigned height) = 0;
		virtual void showWindow(WindowHandle handle) = 0;
		virtual void makeCurrent(WindowHandle handle) = 0;
		virtual void endDraw(WindowHandle handle) = 0;

		Memory::IMemoryBlock* owningBlock() { return _owningBlock; }
	};

	YAGE_API bool initialize(const eastl::string& name, Memory::IMemoryBlock* block, SSubsystemParams& params);
	YAGE_API void shutdown();

	YAGE_API ISubsystem& getSubsystem();
}

#endif // YAGE_SUBSYSTEM_H