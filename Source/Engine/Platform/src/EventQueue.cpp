//
// Created by Bentoo on 2017-02-14.
//

#include "Platform/EventQueue.h"
#include "Platform/Platform.h"
#include "Platform/Subsystem/ISubsystem.h"
#include "Platform/Window.h"
#include "Platform/Logger.h"

namespace Core
{
	Event::Event()
		: window(nullptr)
	{ }

	Event::Event(EventType::Enum _type,
				 yage::platform::WindowHandle _windowPtr)
		: type(_type), window(_windowPtr)
	{ }

	Event::Event(EventType::Enum _type,
				 Input::InputEvent* _inputData,
				 yage::platform::WindowHandle _windowPtr)
		: type(_type), inputData(*_inputData), window(_windowPtr)
	{ }

	Event::Event(EventType::Enum _type,
				 Core::WindowEvent* _windowData,
				 yage::platform::WindowHandle _windowPtr)
		: type(_type), windowData(*_windowData), window(_windowPtr)
	{ }

	void EventQueue::initializeImpl(Memory::IMemoryBlock &memory)
	{
		if (_memory != nullptr)
			return;

		_memory 	= &memory;
		_eventQueue = YAGE_CREATE_NEW(memory, Utils::List<Event>)();// #NewAlloc  memory);
		_pollIndex	= 0;
	}

	bool EventQueue::initialize(Memory::IMemoryBlock& memory)
	{
		bool result = true;

		get().initializeImpl(memory);

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
		if (_eventQueue->empty())
		{
			yage::platform::getSubsystem().pollEvents(this);
		}

		if (!_eventQueue->empty()
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

	void EventQueue::enqueueInputEvent(Input::InputEvent& data, yage::platform::WindowHandle window)
	{
		YAGE_ASSERT(get()._eventQueue != nullptr, "EventQueue : Unable to enqueue event before initialization!");
		get()._eventQueue->emplace_back(EventType::INPUT, &data, window);
	}

	void EventQueue::enqueueWindowEvent(Core::WindowEvent& data, yage::platform::WindowHandle window)
	{
		YAGE_ASSERT(get()._eventQueue != nullptr, "EventQueue : Unable to enqueue event before initialization!");
		get()._eventQueue->emplace_back(EventType::WINDOW, &data, window);
	}

	void EventQueue::enqueueAppEvent()
	{
		YAGE_ASSERT(get()._eventQueue != nullptr, "EventQueue : Unable to enqueue event before initialization!");
		get()._eventQueue->emplace_back(EventType::APP, nullptr);
	}

	bool EventQueue::registerWindow(Core::Window* window)
	{
		if(window == nullptr)
			return false;

		return true;
	}

}
