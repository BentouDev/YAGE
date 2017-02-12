//
// Created by bentoo on 06.02.17.
//

#ifndef YAGE_MESSAGEDISPATCHER_H
#define YAGE_MESSAGEDISPATCHER_H

#include <map>

class GLFWwindow;
class GLFWmonitor;

namespace Core
{
	class Engine;
	class Window;

	struct KeyEventData
	{
		KeyEventData(std::int32_t keycode, std::int32_t  scancode, std::int32_t mods, std::int32_t state)
			: Keycode(keycode), Scancode(scancode), Mods(mods), State(state)
		{ }

		std::int32_t Keycode;
		std::int32_t Scancode;
		std::int32_t Mods;
		std::int32_t State;
	};

	class MessageDispather
	{
	private:
				Engine&					_engine;
				Memory::IMemoryBlock&	_memory;
		static	MessageDispather*		_instance;

		std::map<GLFWwindow*, Window::handle_t> _windowMap;

		void setupGlfwEvents();
		void setupGlfwEventsForWindow(const Window& window);

		// Misc
		static void ErrorCallback(int code, const char* description);
		static void MonitorCallback(GLFWmonitor* monitor, int status);

		// Joystick
		static void JoystickCallback(int joy, int event);

		// Keyboard
		static void CharCallback(GLFWwindow* window, unsigned int codepoint);
		static void CharModCallback(GLFWwindow* window, unsigned int codepoint, int mods);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		// Mouse
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
		static void CursorEnterCallback(GLFWwindow* window, int entered);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		// Window
		static void DropCallback(GLFWwindow* window, int count, const char** paths);
		static void WindowCloseCallback(GLFWwindow* window);
		static void WindowRefreshCallback(GLFWwindow* window);
		static void WindowFocusCallback(GLFWwindow* window, int maximized);
		static void WindowMaximizeCallback(GLFWwindow* window, int maximized);
		static void WindowIconifyCallback(GLFWwindow* window, int iconified);
		static void WindowPosCallback(GLFWwindow* window, int xpos, int ypos);
		static void WindowSizeCallback(GLFWwindow* window, int width, int height);
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

	public:
		explicit MessageDispather(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~MessageDispather();

		void initialize();
		void registerWindow(const Window &window);
	};
}

#endif //YAGE_MESSAGEDISPATCHER_H
