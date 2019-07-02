#include "GLFW_Subsystem.h"

#include "Platform/Graphics/OpenGL/OpenGLBase.h"
#include "Platform/Logger.h"
#include "Platform/EventQueue.h"

#include <GLFW/glfw3.h>

namespace yage::platform::glfw
{
	GLFW::GLFW(Memory::IMemoryBlock* block, SSubsystemParams& params)
		: ISubsystem(block, params)
	{

	}

	auto GLFW::getQueue() -> Core::EventQueue*
	{
		return _eventQueue;
	}

	bool GLFW::initialize()
	{
		if (!glfwInit())
		{
			Core::Logger::error("GLFW : unable to initialize!");
			return false;
		}

		// todo: pick from config
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, gl::TRUE_);

		// Set callbacks
		registerGlobalCallbacks();

		return true;
	}

	void GLFW::shutdown()
	{
		glfwTerminate();
	}

	void GLFW::pollEvents(Core::EventQueue*)
	{
		glfwPollEvents();
	}

	double GLFW::getCurrentTime()
	{
		return glfwGetTime();
	}

	void GLFW::makeCurrent(WindowHandle handle)
	{
		glfwMakeContextCurrent(reinterpret_cast<GLFWwindow*>(handle));
	}

	void GLFW::endDraw(WindowHandle handle)
	{
		glfwSwapBuffers(reinterpret_cast<GLFWwindow*>(handle));
	}

	const char* GLFW::getDeviceName(DeviceHandle handle)
	{
		return glfwGetJoystickName(handle);
	}

	bool GLFW::isDevicePresent(DeviceHandle handle)
	{
		return glfwJoystickPresent(handle);
	}

    WindowHandle GLFW::createWindow(const SWindowParams& params)
    {
        GLFWwindow* handle = glfwCreateWindow(params.width, params.height, params.name.c_str(), nullptr, nullptr);
        if (handle == nullptr)
        {
            Core::Logger::error("GLFW : Unable to create window!");
            return nullptr;
        }

        glfwSetWindowUserPointer(handle, this);
        registerWindowCallbacks(handle);

        return handle;
    }

	bool GLFW::destroyWindow(WindowHandle handle)
	{
#ifdef _MSC_VER
		glfwDestroyWindow(reinterpret_cast<GLFWwindow*>(handle));
#endif
		return true;
	}

	void GLFW::resizeWindow(WindowHandle handle, unsigned width, unsigned height)
	{
		glfwSetWindowSize(reinterpret_cast<GLFWwindow*>(handle), width, height);
	}

	void GLFW::showWindow(WindowHandle handle)
	{
		glfwShowWindow(reinterpret_cast<GLFWwindow*>(handle));
	}
}

namespace yage::platform::glfw::eventCallbacks
{
	// Misc
	void ErrorCallback(int code, const char* description);
	void MonitorCallback(GLFWmonitor* monitor, int status);

	// Joystick
	void JoystickCallback(int joy, int status);

	// Keyboard
	void CharCallback(GLFWwindow* window, unsigned int codepoint);
	void CharModCallback(GLFWwindow* window, unsigned int codepoint, int mods);
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// Mouse
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	void CursorEnterCallback(GLFWwindow* window, int entered);
	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	// Window
	void DropCallback(GLFWwindow* window, int count, const char** paths);
	void WindowCloseCallback(GLFWwindow* window);
	void WindowRefreshCallback(GLFWwindow* window);
	void WindowFocusCallback(GLFWwindow* window, int maximized);
	void WindowMaximizeCallback(GLFWwindow* window, int maximized);
	void WindowIconifyCallback(GLFWwindow* window, int iconified);
	void WindowPosCallback(GLFWwindow* window, int xpos, int ypos);
	void WindowSizeCallback(GLFWwindow* window, int width, int height);
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

	// Problems :
	//  - events for joy
	//  - delta in mouse move
	//  - mouse button code (its from 0)
}

void yage::platform::glfw::GLFW::registerWindowCallbacks(GLFWwindow* window)
{
	glfwSetCharCallback(window, &eventCallbacks::CharCallback);
	glfwSetCharModsCallback(window, &eventCallbacks::CharModCallback);
	glfwSetKeyCallback(window, &eventCallbacks::KeyCallback);

	glfwSetScrollCallback(window, &eventCallbacks::ScrollCallback);
	glfwSetCursorEnterCallback(window, &eventCallbacks::CursorEnterCallback);
	glfwSetCursorPosCallback(window, &eventCallbacks::CursorPosCallback);
	glfwSetMouseButtonCallback(window, &eventCallbacks::MouseButtonCallback);

	glfwSetDropCallback(window, &eventCallbacks::DropCallback);
	glfwSetWindowCloseCallback(window, &eventCallbacks::WindowCloseCallback);
	glfwSetWindowRefreshCallback(window, &eventCallbacks::WindowRefreshCallback);
	glfwSetWindowFocusCallback(window, &eventCallbacks::WindowFocusCallback);
	glfwSetWindowIconifyCallback(window, &eventCallbacks::WindowIconifyCallback);
	glfwSetWindowPosCallback(window, &eventCallbacks::WindowPosCallback);
	glfwSetWindowSizeCallback(window, &eventCallbacks::WindowSizeCallback);
	glfwSetFramebufferSizeCallback(window, &eventCallbacks::FramebufferSizeCallback);
}

void yage::platform::glfw::GLFW::registerGlobalCallbacks()
{
	glfwSetJoystickCallback(&eventCallbacks::JoystickCallback);
	glfwSetMonitorCallback(&eventCallbacks::MonitorCallback);
	glfwSetErrorCallback(&eventCallbacks::ErrorCallback);
}

namespace yage::platform::glfw::eventCallbacks
{
	using namespace Core;

	void ErrorCallback(int code, const char* description)
	{
		Logger::critical("GLFW : Error '{}' occured :\n\t{}", code, description);
	}

	void MonitorCallback(GLFWmonitor* monitor, int status)
	{
		auto vod = reinterpret_cast<void*>(monitor);
		auto ptr = reinterpret_cast<std::uintptr_t>(vod);
		Logger::debug("GLFW : Monitor '{}' changed status to '{}'", ptr, status);
	}

	void JoystickCallback(int joy, int status)
	{
		Input::InputEvent event;

		event.type = status == GLFW_CONNECTED ? Input::EventType::CONNECTED : Input::EventType::DISCONNECTED;
		event.connection.deviceId = joy;
		event.connection.deviceType = Input::DeviceType::CONTROLLER;

		// self->getQueue()->enqueueInputEvent(event, nullptr);

		Logger::debug("GLFW : Joystick '{}' changed status to '{}'", joy, status);
	}

	void CharCallback(GLFWwindow*, unsigned int codepoint)
	{
		Logger::trace("CharEvent : codepoint '{}'", codepoint);
	}

	void CharModCallback(GLFWwindow*, unsigned int codepoint, int mods)
	{
		Logger::trace("CharModEvent : codepoint '{}', mods '{}'", codepoint, mods);
	}

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int /*mods*/)
	{
		auto* self = reinterpret_cast<GLFW*>(glfwGetWindowUserPointer(window));

		Input::InputEvent data;

		data.type = Input::EventType::BUTTON;
		data.button.deviceId = -1;
		data.button.keycode = key;
		data.button.scancode = key == GLFW_KEY_UNKNOWN ? scancode : key;
		data.button.deviceType = Input::DeviceType::KEYBOARD;
		data.button.status = action == GLFW_RELEASE ? Input::ButtonStatus::RELEASED : Input::ButtonStatus::PRESSED;

		self->getQueue()->enqueueInputEvent(data, window);

		Logger::trace("KeyEvent : key '{}', scancode '{}', action '{}'", key, scancode, action);
	}

	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		auto* self = reinterpret_cast<GLFW*>(glfwGetWindowUserPointer(window));

		Input::InputEvent data;

		data.type = Input::EventType::AXIS;
		data.axis.deviceId = -1;
		data.axis.x = (float) xoffset;
		data.axis.y = (float) yoffset;
		data.axis.deviceType = Input::DeviceType::MOUSE;
		data.axis.axisId = 1;

		self->getQueue()->enqueueInputEvent(data, window);

		Logger::trace("ScrollEvent : xoffset '{}', yoffset '{}'", xoffset, yoffset);
	}

	void CursorEnterCallback(GLFWwindow*, int entered)
	{
		Logger::trace("CursorEnterEvent : status '{}'", entered);
	}

	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		auto* self = reinterpret_cast<GLFW*>(glfwGetWindowUserPointer(window));

		Input::InputEvent data;

		data.type = Input::EventType::AXIS;
		data.axis.deviceId = -1;
		data.axis.x = xpos;
		data.axis.y = ypos;
		data.axis.deviceType = Input::DeviceType::MOUSE;
		data.axis.axisId = 0;

		self->getQueue()->enqueueInputEvent(data, window);

		Logger::trace("CursorPosEvent : xpos '{}', ypos '{}'", xpos, ypos);
	}

	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		auto* self = reinterpret_cast<GLFW*>(glfwGetWindowUserPointer(window));

		Input::InputEvent data;

		data.type = Input::EventType::BUTTON;
		data.button.deviceId = -1;
		data.button.keycode = button;
		data.button.scancode = button;
		data.button.deviceType = Input::DeviceType::MOUSE;
		data.button.status = action == GLFW_RELEASE ? Input::ButtonStatus::RELEASED : Input::ButtonStatus::PRESSED;

		self->getQueue()->enqueueInputEvent(data, window);

		Logger::trace("MouseButtonEvent : button '{}', action '{}', mods '{}'", button, action, mods);
	}

	void DropCallback(GLFWwindow* window, int /*count*/, const char** /*paths*/)
	{
		auto* self = reinterpret_cast<GLFW*>(glfwGetWindowUserPointer(window));

		Core::WindowEvent data;

		data.type = WindowEventType::DROP;

		self->getQueue()->enqueueWindowEvent(data, window);

		// Logger::debug("WindowDropEvent : window '{}', count '{}'.", window, count);
	}

	void WindowCloseCallback(GLFWwindow* window)
	{
		auto* self = reinterpret_cast<GLFW*>(glfwGetWindowUserPointer(window));

		Core::WindowEvent data;

		data.type = WindowEventType::CLOSE;

		self->getQueue()->enqueueWindowEvent(data, window);

		// Logger::trace("WindowCloseEvent : window '{}'", window);
	}

	void WindowRefreshCallback(GLFWwindow*)
	{

	}

	void WindowFocusCallback(GLFWwindow* window, int /*maximized*/)
	{
		auto* self = reinterpret_cast<GLFW*>(glfwGetWindowUserPointer(window));

		Core::WindowEvent data;

		data.type = WindowEventType::FOCUS;

		self->getQueue()->enqueueWindowEvent(data, window);

		// Logger::trace("WindowFocusEvent : window '{}'", window);
	}

	void WindowIconifyCallback(GLFWwindow* window, int /*iconified*/)
	{
		auto* self = reinterpret_cast<GLFW*>(glfwGetWindowUserPointer(window));

		Core::WindowEvent data;

		data.type = WindowEventType::ICONIFY;

		self->getQueue()->enqueueWindowEvent(data, window);

		// Logger::trace("WindowIconifyEvent : window '{}'", window);
	}

	void WindowPosCallback(GLFWwindow* window, int xpos, int ypos)
	{
		auto* self = reinterpret_cast<GLFW*>(glfwGetWindowUserPointer(window));

		Core::WindowEvent data;

		data.type = WindowEventType::MOVE;
		data.coord.x = xpos;
		data.coord.y = ypos;

		self->getQueue()->enqueueWindowEvent(data, window);

		// Logger::trace("WindowMoveEvent : window '{}'", window);
	}

	void WindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		auto* self = reinterpret_cast<GLFW*>(glfwGetWindowUserPointer(window));

		Core::WindowEvent data;

		data.type = WindowEventType::RESIZE;
		data.coord.x = width;
		data.coord.y = height;

		self->getQueue()->enqueueWindowEvent(data, window);

		Logger::info("WindowSizeEvent : width '{}', height '{}'", width, height);
	}

	void FramebufferSizeCallback(GLFWwindow*, int width, int height)
	{
		Logger::debug("FramebufferSizeEvent : width '{}', height '{}'", width, height);
	}
}