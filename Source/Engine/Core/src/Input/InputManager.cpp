//
// Created by bentoo on 13.02.17.
//

#include <Core/Input/InputEvent.h>
#include <Core/EventQueue.h>
#include "Core/Input/InputManager.h"
#include "Core/Input/ControlScheme.h"
#include "Core/Input/InputEvent.h"
#include "Core/Platform.h"
#include "Core/Logger.h"

namespace Core
{
	InputDevice::InputDevice(Memory::IMemoryBlock& memory, std::int32_t deviceId)
		: name(memory, glfwGetJoystickName(deviceId)),
		  scheme(nullptr), type(Input::DeviceType::CONTROLLER)
	{ }

	InputDevice::~InputDevice()
	{ }

	InputManager::~InputManager()
	{
		for(auto itr : _controllerIdMap)
		{
			Memory::Delete(_memory, itr.second);
			itr.second = nullptr;
		}
	}

	InputDevice* InputManager::getDeviceForId(std::int32_t id)
	{
		InputDevice*	result	= nullptr;
		auto			itr		= _controllerIdMap.find(id);

		if(itr != _controllerIdMap.end())
		{
			result = itr->second;
		}

		return result;
	}

	InputDevice* InputManager::getDeviceForEvent(const Input::InputEvent& event)
	{
		InputDevice* result = nullptr;

		switch(event.deviceType)
		{
			case Input::DeviceType::MOUSE:
				result = &Mouse;
				break;
			case Input::DeviceType::KEYBOARD:
				result = &Keyboard;
				break;
			case Input::DeviceType::CONTROLLER:
				result = getDeviceForId(event.deviceId);
				break;
			default:
				Logger::warn("InputManager : Unknown device '%zu'", event.deviceType);
				break;
		}

		return result;
	}

	auto InputManager::getSchemeForDevice(InputDevice* device) -> Input::ControlScheme*
	{
		if(device == nullptr)
			return nullptr;

		return device->scheme != nullptr ? device->scheme : _currentScheme;
	}

	void InputManager::onDeviceDisconnected(const Input::ConnectionEvent& event)
	{
		auto device = getDeviceForId(event.deviceId);
		if(device != nullptr)
		{
			Core::Logger::debug("Controller : Disconnected '{}', named '{}'.",
								device->id, device->name.c_str());

			_controllerIdMap.erase(device->id);
			Memory::Delete(_memory, device);
		}
	}

	void InputManager::onDeviceConnected(const Input::ConnectionEvent& event)
	{
		if(event.deviceId == Input::DeviceType::CONTROLLER
	   	&& glfwJoystickPresent(event.deviceId))
		{
			auto inputDevice = YAGE_CREATE_NEW(_memory, InputDevice)(_memory, event.deviceId);

			Core::Logger::debug("Controller : Connected '{}', named '{}'.",
								event.deviceId, inputDevice->name.c_str());

			_controllerIdMap[event.deviceId] = inputDevice;
		}
	}

	void InputManager::onKey(InputDevice* device, std::int32_t scancode, std::int32_t state, Core::GameTime& time)
	{
		Input::ControlScheme* scheme = getSchemeForDevice(device);
		if(scheme != nullptr)
		{
			scheme->updateButtonByScancode(device->type, scancode, state, time);
		}
	}

	void InputManager::onAxis(InputDevice* device, std::int32_t axis, std::int32_t x, std::int32_t y, Core::GameTime& time)
	{
		Input::ControlScheme* scheme = getSchemeForDevice(device);
		if(scheme != nullptr)
		{
		//	scheme->updateAxisById(device->type, axis, x, y, time);
		}
	}

	void InputManager::handleInputEvent(const Core::Event& event, Core::GameTime& time)
	{
		auto input  = event.inputData;
		auto device = getDeviceForEvent(input);

		switch(input.type)
		{
			case Input::EventType::BUTTON:
				onKey(device, input.button.scancode, input.button.status, time);
				break;
			case Input::EventType::AXIS:
				onAxis(device, input.axis.axisId, input.axis.x, input.axis.y, time);
				break;
			case Input::EventType::CONNECTED:
				onDeviceConnected(input.connection);
				break;
			case Input::EventType::DISCONNECTED:
				onDeviceDisconnected(input.connection);
				break;
		}
	}
}
