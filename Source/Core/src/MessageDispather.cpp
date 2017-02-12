//
// Created by bentoo on 06.02.17.
//

#include "Core/Window.h"
#include "Core/MessageDispather.h"
#include "Core/WindowManager.h"
#include "Core/Platform.h"
#include "Core/Engine.h"
#include "Core/Logger.h"

Core::MessageDispather* Core::MessageDispather::_instance = nullptr;

namespace Core
{
	MessageDispather::MessageDispather(Core::Engine& engine, Memory::IMemoryBlock& memory)
		: _engine(engine), _memory(memory)
	{ }

	MessageDispather::~MessageDispather()
	{ }

	void MessageDispather::initialize()
	{
		_instance = this;
		setupGlfwEvents();

		Logger::info("MessageDispatcher : Message handlers initialized.");
	}

	void MessageDispather::registerWindow(const Window &window)
	{
		setupGlfwEventsForWindow(window);
	}

	void MessageDispather::setupGlfwEvents()
	{
		glfwSetJoystickCallback       (&MessageDispather::JoystickCallback);
		glfwSetErrorCallback          (&MessageDispather::ErrorCallback);
		glfwSetMonitorCallback        (&MessageDispather::MonitorCallback);
	}

	void MessageDispather::setupGlfwEventsForWindow(const Window& window)
	{
		glfwSetCharCallback           (window.hWindow, &MessageDispather::CharCallback);
		glfwSetCharModsCallback       (window.hWindow, &MessageDispather::CharModCallback);
		glfwSetKeyCallback            (window.hWindow, &MessageDispather::KeyCallback);

		glfwSetScrollCallback         (window.hWindow, &MessageDispather::ScrollCallback);
		glfwSetCursorEnterCallback    (window.hWindow, &MessageDispather::CursorEnterCallback);
		glfwSetCursorPosCallback      (window.hWindow, &MessageDispather::CursorPosCallback);
		glfwSetMouseButtonCallback    (window.hWindow, &MessageDispather::MouseButtonCallback);

		glfwSetDropCallback           (window.hWindow, &MessageDispather::DropCallback);
		glfwSetWindowCloseCallback    (window.hWindow, &MessageDispather::WindowCloseCallback);
		glfwSetWindowRefreshCallback  (window.hWindow, &MessageDispather::WindowRefreshCallback);
		glfwSetWindowFocusCallback    (window.hWindow, &MessageDispather::WindowFocusCallback);
		glfwSetWindowIconifyCallback  (window.hWindow, &MessageDispather::WindowIconifyCallback);
		glfwSetWindowMaximizeCallback (window.hWindow, &MessageDispather::WindowMaximizeCallback);
		glfwSetWindowPosCallback      (window.hWindow, &MessageDispather::WindowPosCallback);
		glfwSetWindowSizeCallback     (window.hWindow, &MessageDispather::WindowSizeCallback);
		glfwSetFramebufferSizeCallback(window.hWindow, &MessageDispather::FramebufferSizeCallback);
	}

	void MessageDispather::ErrorCallback(int code, const char* description)
	{
		Logger::error("GLFW : Error '{}' occured :\n\t{}", code, description);
	}

	void MessageDispather::MonitorCallback(GLFWmonitor* monitor, int status)
	{
		auto vod = reinterpret_cast<void*>(monitor);
		auto ptr = reinterpret_cast<std::uintptr_t>(vod);
		Logger::error("GLFW : Monitor '{}' changed status to '{}'", ptr, status);
	}

	void MessageDispather::JoystickCallback(int joy, int event)
	{
		Logger::error("GLFW : Joystick '{}' changed status to '{}'", joy, event);
	}

	void MessageDispather::CharCallback(GLFWwindow* window, unsigned int codepoint)
	{
		Logger::info("CharEvent : codepoint '{}'", codepoint);
	}

	void MessageDispather::CharModCallback(GLFWwindow *window, unsigned int codepoint, int mods)
	{
		Logger::info("CharModEvent : codepoint '{}', mods '{}'", codepoint, mods);
	}

	void MessageDispather::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Logger::info("KeyEvent : key '{}', scancode '{}', action '{}'", key, scancode, action);

		/*auto ptr = _instance->_windowMap.find(window);
		if(ptr != _instance->_windowMap.end())
		{
			auto windowHandle = ptr->second;
		}*/
	}

	void MessageDispather::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
	{
		Logger::info("ScrollEvent : xoffset '{}', yoffset '{}'", xoffset, yoffset);
	}

	void MessageDispather::CursorEnterCallback(GLFWwindow *window, int entered)
	{
		Logger::info("CursorEnterEvent : status '{}'", entered);
	}

	void MessageDispather::CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
	{
		Logger::info("CursorPosEvent : xpos '{}', ypos '{}'", xpos, ypos);
	}

	void MessageDispather::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
	{
		Logger::info("MouseButtonEvent : button '{}', action '{}', mods '{}'", button, action, mods);
	}

	void MessageDispather::DropCallback(GLFWwindow *window, int count, const char **paths)
	{

	}

	void MessageDispather::WindowCloseCallback(GLFWwindow *window)
	{
		Logger::info("WindowCloseEvent : nothing");
	}

	void MessageDispather::WindowRefreshCallback(GLFWwindow *window)
	{

	}

	void MessageDispather::WindowFocusCallback(GLFWwindow *window, int maximized)
	{

	}

	void MessageDispather::WindowIconifyCallback(GLFWwindow *window, int iconified)
	{

	}

	void MessageDispather::WindowMaximizeCallback(GLFWwindow *window, int maximized)
	{

	}

	void MessageDispather::WindowPosCallback(GLFWwindow *window, int xpos, int ypos)
	{

	}

	void MessageDispather::WindowSizeCallback(GLFWwindow *window, int width, int height)
	{
		Logger::info("WindowSizeEvent : width '{}', height '{}'", width, height);
	}

	void MessageDispather::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
	{
		Logger::info("FramebufferSizeEvent : width '{}', height '{}'", width, height);
	}
}
