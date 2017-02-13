//
// Created by bentoo on 06.02.17.
//

#include "Core/Window.h"
#include "Core/Input/MessageDispather.h"
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

		Logger::info("MessageDispatcher : Message handlers initialized.");
	}

	void MessageDispather::registerWindow(const Window &window)
	{

	}
}
