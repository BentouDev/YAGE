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

	public:
		explicit MessageDispather(Core::Engine& engine, Memory::IMemoryBlock& memory);
		virtual ~MessageDispather();

		void initialize();
		void registerWindow(const Window &window);
	};
}

#endif //YAGE_MESSAGEDISPATCHER_H
