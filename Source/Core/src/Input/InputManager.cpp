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
	InputDevice::InputDevice(SDL_GameController* controller)
		: //_name(SDL_GameControllerName(controller)),
		  _hController(controller), _scheme(nullptr)
	{ }

	InputDevice::~InputDevice()
	{
		if(_hController != nullptr)
		{
		//	SDL_GameControllerClose(_hController);
		}
	}

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

	auto InputManager::getSchemeForDevice(InputDevice* device) -> Input::ControlScheme*
	{
		if(device == nullptr)
			return nullptr;

		return device->_scheme != nullptr ? device->_scheme : _currentScheme;
	}

	void InputManager::onDeviceDisconnected(InputDevice* device)
	{
		if(device != nullptr)
		{
			Core::Logger::debug("Controller : Disconnected '{}', named '{}'.", device->_id, device->_name);

			_controllerIdMap.erase(device->_id);
			Memory::Delete(_memory, device);
		}
	}

	void InputManager::onDeviceConnected(InputDevice* device)
	{
		if(device != nullptr)
		{
			/*SDL_GameController* controller = SDL_GameControllerOpen(event.cdevice.which);
			if(controller != nullptr)
			{
				auto inputDevice = YAGE_CREATE_NEW(_memory, InputDevice)(controller);

				Core::Logger::debug("Controller : Connected '{}', named '{}'.",
									event.cdevice.which, inputDevice->_name);

				SDL_Joystick	*joy		= SDL_GameControllerGetJoystick( controller );
				int				instanceID	= SDL_JoystickInstanceID( joy );

				_controllerIdMap[instanceID] = inputDevice;
			}*/
		}
	}

	void InputManager::onKey(InputDevice* device, std::int32_t scancode, std::int32_t state, Core::GameTime& time)
	{
		Input::ControlScheme* scheme = getSchemeForDevice(device);
		if(scheme != nullptr)
		{
			scheme->updateButtonByScancode(scancode, state, time);
		}
	}

	void InputManager::onAxis(InputDevice* device, std::int32_t axis, std::int32_t x, std::int32_t y, Core::GameTime& time)
	{
		Input::ControlScheme* scheme = getSchemeForDevice(device);
		if(scheme != nullptr)
		{
		//	scheme->updateAxisById(axis, x, y);
		}
	}

	void InputManager::handleInputEvent(const Core::Event& event, Core::GameTime& time)
	{
		auto input  = event.inputData;
		auto device = input.isKeyboardAndMouseEvent() ? &MouseAndKeyboard : getDeviceForId(input.getDeviceId());
		switch(input.type)
		{
			case Input::EventType::BUTTON:
				onKey(device, input.button.scancode, input.button.status, time);
				break;
			case Input::EventType::AXIS:
				onAxis(device, input.axis.axisId, input.axis.x, input.axis.y, time);
				break;
			case Input::EventType::CONNECTED:
				onDeviceConnected(device);
				break;
			case Input::EventType::DISCONNECTED:
				onDeviceDisconnected(device);
				break;
		}
	}
}
