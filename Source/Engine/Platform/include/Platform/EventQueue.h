//
// Created by Bentoo on 2017-02-14.
//

#ifndef YAGE_EVENTQUEUE_H
#define YAGE_EVENTQUEUE_H

#include <Utils/MemoryBlock.h>
#include <Utils/List.h>

#include "Platform/Platform.h"
#include "Platform/InputEvent.h"
#include "Platform/WindowEvent.h"

namespace Input
{
	struct InputEvent;
}

namespace Core
{
	class Window;
	struct WindowEvent;

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
			   Core::WindowEvent* _windowData,
               yage::platform::WindowHandle _windowPtr);

		Event(EventType::Enum _type,
			  Input::InputEvent* _inputData,
			  yage::platform::WindowHandle _windowPtr);

		Event(EventType::Enum _type,
			  yage::platform::WindowHandle _windowPtr);

		 Event();
		~Event() = default;

		EventType::Enum type;

		union
		{
			Input::InputEvent inputData;
			Core::WindowEvent windowData;
		};

		yage::platform::WindowHandle window;
	};

	class EventQueue
	{
		Memory::IMemoryBlock*	_memory;
		Utils::List<Event>*		_eventQueue;
		std::int32_t 			_pollIndex;

		void initializeImpl(Memory::IMemoryBlock& memory);
		bool poolEventImpl (Event* event);

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

		void enqueueWindowEvent(Core::WindowEvent& data, yage::platform::WindowHandle window);
		void enqueueInputEvent(Input::InputEvent& data, yage::platform::WindowHandle window);
		void enqueueAppEvent();
	};
}

#endif //YAGE_EVENTQUEUE_H
