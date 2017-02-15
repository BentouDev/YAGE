//
// Created by Bentoo on 2017-02-14.
//

#include "Core/EventQueue.h"
#include "Core/Platform.h"
#include "Core/Window.h"
#include "Core/Logger.h"

namespace Core
{
	Event::Event()
		: window(nullptr)
	{ }

	Event::Event(EventType::Enum _type,
				 GLFWwindow* _windowPtr)
		: type(_type), window(_windowPtr)
	{ }

	Event::Event(EventType::Enum _type,
				 Input::InputEvent _inputData,
				 GLFWwindow* _windowPtr)
			: type(_type), inputData(_inputData), window(_windowPtr)
	{ }

	Event::Event(EventType::Enum _type,
				 Core::WindowEvent _windowData,
				 GLFWwindow* _windowPtr)
		: type(_type), windowData(_windowData), window(_windowPtr)
	{ }

	void EventQueue::initializeImpl(Memory::IMemoryBlock &memory)
	{
		if(_memory != nullptr)
			return;

		_memory 	= &memory;
		_eventQueue	= YAGE_CREATE_NEW(memory, Utils::List<Event>)(memory);
		_pollIndex	= 0;
	}

	bool EventQueue::initialize(Memory::IMemoryBlock& memory)
	{
		bool result = true;

		get().initializeImpl(memory);

		glfwSetJoystickCallback(&EventQueue::JoystickCallback);
		glfwSetMonitorCallback (&EventQueue::MonitorCallback);

		Logger::debug("EventQueue : Event size = '{}'", sizeof(Event));
		Logger::debug("EventQueue : InputEvent size = '{}'", sizeof(Input::InputEvent));
		Logger::debug("EventQueue : WindowEvent size = '{}'", sizeof(Core::WindowEvent));

		return result;
	}

	void EventQueue::destroy()
	{
		Memory::Delete(*get()._memory, get()._eventQueue);
		get()._memory = nullptr;
	}

	bool EventQueue::poolEventImpl(Event* event)
	{
		if(_eventQueue->empty())
		{
			glfwPollEvents();
		}

		if(!_eventQueue->empty()
	   	&&  _pollIndex < _eventQueue->size())
		{
			*event = (*_eventQueue)[_pollIndex];
			_pollIndex++;

			return true;
		}

		_pollIndex = 0;
		_eventQueue->clear();

		return false;
	}

	bool EventQueue::pollEvent(Event *event)
	{
		return get().poolEventImpl(event);
	}

	void EventQueue::enqueueInputEvent(Input::InputEvent& data, GLFWwindow* window)
	{
		YAGE_ASSERT(get()._eventQueue != nullptr, "EventQueue : Unable to enqueue event before initialization!");
		get()._eventQueue->emplace(EventType::INPUT, data, window);
	}

	void EventQueue::enqueueWindowEvent(Core::WindowEvent& data, GLFWwindow* window)
	{
		YAGE_ASSERT(get()._eventQueue != nullptr, "EventQueue : Unable to enqueue event before initialization!");
		get()._eventQueue->emplace(EventType::WINDOW, data, window);
	}

	void EventQueue::enqueueAppEvent()
	{
		YAGE_ASSERT(get()._eventQueue != nullptr, "EventQueue : Unable to enqueue event before initialization!");
		get()._eventQueue->emplace(EventType::APP, nullptr);
	}

	bool EventQueue::registerWindow(Core::Window* window)
	{
		if(window == nullptr)
			return false;

		glfwSetCharCallback           (window->hWindow, &EventQueue::CharCallback);
		glfwSetCharModsCallback       (window->hWindow, &EventQueue::CharModCallback);
		glfwSetKeyCallback            (window->hWindow, &EventQueue::KeyCallback);

		glfwSetScrollCallback         (window->hWindow, &EventQueue::ScrollCallback);
		glfwSetCursorEnterCallback    (window->hWindow, &EventQueue::CursorEnterCallback);
		glfwSetCursorPosCallback      (window->hWindow, &EventQueue::CursorPosCallback);
		glfwSetMouseButtonCallback    (window->hWindow, &EventQueue::MouseButtonCallback);

		glfwSetDropCallback           (window->hWindow, &EventQueue::DropCallback);
		glfwSetWindowCloseCallback    (window->hWindow, &EventQueue::WindowCloseCallback);
		glfwSetWindowRefreshCallback  (window->hWindow, &EventQueue::WindowRefreshCallback);
		glfwSetWindowFocusCallback    (window->hWindow, &EventQueue::WindowFocusCallback);
		glfwSetWindowIconifyCallback  (window->hWindow, &EventQueue::WindowIconifyCallback);
		glfwSetWindowMaximizeCallback (window->hWindow, &EventQueue::WindowMaximizeCallback);
		glfwSetWindowPosCallback      (window->hWindow, &EventQueue::WindowPosCallback);
		glfwSetWindowSizeCallback     (window->hWindow, &EventQueue::WindowSizeCallback);
		glfwSetFramebufferSizeCallback(window->hWindow, &EventQueue::FramebufferSizeCallback);

		return true;
	}

	void EventQueue::MonitorCallback(GLFWmonitor* monitor, int status)
	{
		auto vod = reinterpret_cast<void*>(monitor);
		auto ptr = reinterpret_cast<std::uintptr_t>(vod);
		Logger::debug("GLFW : Monitor '{}' changed status to '{}'", ptr, status);
	}

	void EventQueue::JoystickCallback(int joy, int status)
	{
		Input::InputEvent event;

		event.type                  = status == GLFW_CONNECTED ? Input::EventType::CONNECTED : Input::EventType::DISCONNECTED;
		event.connection.deviceId   = joy;
		event.connection.deviceType = Input::DeviceType::CONTROLLER;

		get().enqueueInputEvent(event, nullptr);

		Logger::debug("GLFW : Joystick '{}' changed status to '{}'", joy, status);
	}

	void EventQueue::CharCallback(GLFWwindow* window, unsigned int codepoint)
	{
		Logger::trace("CharEvent : codepoint '{}'", codepoint);
	}

	void EventQueue::CharModCallback(GLFWwindow* window, unsigned int codepoint, int mods)
	{
		Logger::trace("CharModEvent : codepoint '{}', mods '{}'", codepoint, mods);
	}

	void EventQueue::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Input::InputEvent data;

		data.type              = Input::EventType::BUTTON;
		data.button.deviceId   = -1;
		data.button.keycode    = key;
		data.button.scancode   = key == GLFW_KEY_UNKNOWN ? scancode : key;
		data.button.deviceType = Input::DeviceType::KEYBOARD;
		data.button.status     = action == GLFW_RELEASE ? Input::ButtonStatus::RELEASED : Input::ButtonStatus::PRESSED;

		get().enqueueInputEvent(data, window);

		Logger::trace("KeyEvent : key '{}', scancode '{}', action '{}'", key, scancode, action);
	}

	void EventQueue::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Input::InputEvent data;

		data.type              = Input::EventType::AXIS;
		data.axis.deviceId     = -1;
		data.axis.x            = xoffset;
		data.axis.y            = yoffset;
		data.axis.deviceType   = Input::DeviceType::MOUSE;
		data.axis.axisId       = 1;

		get().enqueueInputEvent(data, window);

		Logger::trace("ScrollEvent : xoffset '{}', yoffset '{}'", xoffset, yoffset);
	}

	void EventQueue::CursorEnterCallback(GLFWwindow* window, int entered)
	{
		Logger::trace("CursorEnterEvent : status '{}'", entered);
	}

	void EventQueue::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		Input::InputEvent data;

		data.type              = Input::EventType::AXIS;
		data.axis.deviceId     = -1;
		data.axis.x            = xpos;
		data.axis.y            = ypos;
		data.axis.deviceType   = Input::DeviceType::MOUSE;
		data.axis.axisId       = 0;

		get().enqueueInputEvent(data, window);

		Logger::trace("CursorPosEvent : xpos '{}', ypos '{}'", xpos, ypos);
	}

	void EventQueue::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		Input::InputEvent data;

		data.type              = Input::EventType::BUTTON;
		data.button.deviceId   = -1;
		data.button.keycode    = button;
		data.button.scancode   = button;
		data.button.deviceType = Input::DeviceType::MOUSE;
		data.button.status     = action == GLFW_RELEASE ? Input::ButtonStatus::RELEASED : Input::ButtonStatus::PRESSED;

		get().enqueueInputEvent(data, window);

		Logger::trace("MouseButtonEvent : button '{}', action '{}', mods '{}'", button, action, mods);
	}

	void EventQueue::DropCallback(GLFWwindow* window, int count, const char **paths)
	{
		Core::WindowEvent data;

		data.type = WindowEventType::DROP;

		get().enqueueWindowEvent(data, window);

		// Logger::debug("WindowDropEvent : window '{}', count '{}'.", window, count);
	}

	void EventQueue::WindowCloseCallback(GLFWwindow* window)
	{
		Core::WindowEvent data;

		data.type = WindowEventType::CLOSE;

		get().enqueueWindowEvent(data, window);

		// Logger::trace("WindowCloseEvent : window '{}'", window);
	}

	void EventQueue::WindowRefreshCallback(GLFWwindow* window)
	{

	}

	void EventQueue::WindowFocusCallback(GLFWwindow* window, int maximized)
	{
		Core::WindowEvent data;

		data.type = WindowEventType::FOCUS;

		get().enqueueWindowEvent(data, window);

		// Logger::trace("WindowFocusEvent : window '{}'", window);
	}

	void EventQueue::WindowIconifyCallback(GLFWwindow* window, int iconified)
	{
		Core::WindowEvent data;

		data.type = WindowEventType::ICONIFY;

		get().enqueueWindowEvent(data, window);

		// Logger::trace("WindowIconifyEvent : window '{}'", window);
	}

	void EventQueue::WindowMaximizeCallback(GLFWwindow* window, int maximized)
	{
		Core::WindowEvent data;

		data.type = WindowEventType::MAXIMIZE;

		get().enqueueWindowEvent(data, window);

		// Logger::trace("WindowMaximizeEvent : window '{}'", window);
	}

	void EventQueue::WindowPosCallback(GLFWwindow* window, int xpos, int ypos)
	{
		Core::WindowEvent data;

		data.type = WindowEventType::MOVE;
		data.coord.x = xpos;
		data.coord.y = ypos;

		get().enqueueWindowEvent(data, window);

		// Logger::trace("WindowMoveEvent : window '{}'", window);
	}

	void EventQueue::WindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		Core::WindowEvent data;

		data.type = WindowEventType::RESIZE;
		data.coord.x = width;
		data.coord.y = height;

		get().enqueueWindowEvent(data, window);

		Logger::info("WindowSizeEvent : width '{}', height '{}'", width, height);
	}

	void EventQueue::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		Logger::debug("FramebufferSizeEvent : width '{}', height '{}'", width, height);
	}
}
