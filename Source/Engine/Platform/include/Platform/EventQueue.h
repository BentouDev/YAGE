//
// Created by Bentoo on 2017-02-14.
//

#ifndef YAGE_EVENTQUEUE_H
#define YAGE_EVENTQUEUE_H

#include <Utils/MemoryBlock.h>
#include <Utils/List.h>

#include "Platform/InputEvent.h"
#include "Platform/WindowEvent.h"

struct GLFWwindow;
struct GLFWmonitor;

namespace Input
{
	union InputEvent;
}

namespace Core
{
	class Window;
	union WindowEvent;

	namespace EventType
	{
		enum Enum : std::int8_t
		{
			WINDOW,
			INPUT,
			APP
		};
	}

	struct Event
	{
		 Event(EventType::Enum _type,
			   Core::WindowEvent _windowData,
			   GLFWwindow* _windowPtr);

		Event(EventType::Enum _type,
			  Input::InputEvent _inputData,
			  GLFWwindow* _windowPtr);

		Event(EventType::Enum _type,
			  GLFWwindow* _windowPtr);

		 Event();
		~Event() = default;

		EventType::Enum		type;

		union
		{
			Input::InputEvent	inputData;
			Core::WindowEvent	windowData;
		};

		GLFWwindow*			window;
	};

	class EventQueue
	{
		Memory::IMemoryBlock*	_memory;
		Utils::List<Event>*		_eventQueue;
		std::int32_t 			_pollIndex;

		void initializeImpl(Memory::IMemoryBlock& memory);
		bool poolEventImpl (Event* event);

	private:
		// Misc
		static void MonitorCallback        (GLFWmonitor* monitor, int status);

		// Joystick
		static void JoystickCallback       (int joy, int status);

		// Keyboard
		static void CharCallback           (GLFWwindow* window, unsigned int codepoint);
		static void CharModCallback        (GLFWwindow* window, unsigned int codepoint, int mods);
		static void KeyCallback            (GLFWwindow* window, int key, int scancode, int action, int mods);

		// Mouse
		static void ScrollCallback         (GLFWwindow* window, double xoffset, double yoffset);
		static void CursorPosCallback      (GLFWwindow* window, double xpos, double ypos);
		static void CursorEnterCallback    (GLFWwindow* window, int entered);
		static void MouseButtonCallback    (GLFWwindow* window, int button, int action, int mods);

		// Window
		static void DropCallback           (GLFWwindow* window, int count, const char** paths);
		static void WindowCloseCallback    (GLFWwindow* window);
		static void WindowRefreshCallback  (GLFWwindow* window);
		static void WindowFocusCallback    (GLFWwindow* window, int maximized);
		static void WindowMaximizeCallback (GLFWwindow* window, int maximized);
		static void WindowIconifyCallback  (GLFWwindow* window, int iconified);
		static void WindowPosCallback      (GLFWwindow* window, int xpos, int ypos);
		static void WindowSizeCallback     (GLFWwindow* window, int width, int height);
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

		void enqueueWindowEvent(Core::WindowEvent& data, GLFWwindow* window);
		void enqueueInputEvent (Input::InputEvent& data, GLFWwindow* window);
		void enqueueAppEvent   ();

		// Problems :
		//  - events for joy
		//  - delta in mouse move
		//  - mouse button code (its from 0)

	public:
		static EventQueue& get()
		{
			static EventQueue instance;
			return instance;
		}

		static bool pollEvent(Event *event);
		static bool initialize(Memory::IMemoryBlock& memory);
		static bool registerWindow(Core::Window* window);
		static void destroy();
	};
}

#endif //YAGE_EVENTQUEUE_H
