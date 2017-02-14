//
// Created by bentoo on 13.02.17.
//

#include "Core/Input/InputManager.h"
#include "Core/Input/ControlScheme.h"
#include "Core/Platform.h"
#include "Core/Logger.h"

namespace Core
{
	InputDevice::InputDevice(SDL_GameController* controller)
		: _name(SDL_GameControllerName(controller)),
		  _hController(controller), _scheme(nullptr)
	{ }

	InputDevice::~InputDevice()
	{
		if(_hController != nullptr)
		{
			SDL_GameControllerClose(_hController);
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

	void InputManager::onDeviceDisconnected(const SDL_Event &event)
	{
		auto itr = _controllerIdMap.find(event.jdevice.which);
		if(itr != _controllerIdMap.end())
		{
			Core::Logger::debug("Controller : Disconnected '{}', named '{}'.",
								event.cdevice.which, itr->second->_name);

			Memory::Delete(_memory, itr->second);
			_controllerIdMap.erase(itr->first);
		}
	}

	void InputManager::onDeviceConnected(const SDL_Event &event)
	{
		if(SDL_IsGameController(event.cdevice.which))
		{
			SDL_GameController* controller = SDL_GameControllerOpen(event.cdevice.which);
			if(controller != nullptr)
			{
				auto inputDevice = YAGE_CREATE_NEW(_memory, InputDevice)(controller);

				Core::Logger::debug("Controller : Connected '{}', named '{}'.",
									event.cdevice.which, inputDevice->_name);

				SDL_Joystick	*joy		= SDL_GameControllerGetJoystick( controller );
				int				instanceID	= SDL_JoystickInstanceID( joy );

				_controllerIdMap[instanceID] = inputDevice;
			}
		}
	}

	void InputManager::onDeviceRemapped(const SDL_Event &event)
	{
		Core::Logger::debug("Controller : Remapped '{}' of type '{}'.",
							event.cdevice.which, event.cdevice.type);
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

	void InputManager::handleInputEvent(SDL_Event& event, Core::GameTime& time)
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				onKey(&MouseAndKeyboard, event.key.keysym.scancode, event.key.state, time);
				break;
			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_CONTROLLERBUTTONUP:
				onKey(getDeviceForId(event.cbutton.which), event.cbutton.button, event.cbutton.state, time);
				break;
			case SDL_MOUSEMOTION:
				Core::Logger::trace("Mouse : Motion Range X, Y : '{}' x '{}'.",
									event.motion.x, event.motion.y);
				break;
			case SDL_MOUSEWHEEL:
				Core::Logger::debug("Mouse : Wheel Range X, Y : '{}' x '{}'.",
									event.wheel.x, event.wheel.y);
				break;
			case SDL_CONTROLLERDEVICEADDED:
				onDeviceConnected(event);
				break;
			case SDL_CONTROLLERDEVICEREMAPPED:
				onDeviceRemapped(event);
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				onDeviceDisconnected(event);
				break;
		}
	}
}
