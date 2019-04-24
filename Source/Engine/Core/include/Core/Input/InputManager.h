//
// Created by Bentoo on 2017-02-10.
//

#ifndef YAGE_INPUTMANAGER_H
#define YAGE_INPUTMANAGER_H

#include <map>
#include <Utils/MemoryBlock.h>
#include <Utils/String.h>
#include <Platform/DeviceType.h>

#include "Core/GameTime.h"
#include "Core/IManager.h"

struct _SDL_GameController;
typedef struct _SDL_GameController SDL_GameController;

namespace Input
{
	class ControlScheme;
	union InputEvent;
	struct ConnectionEvent;
}

namespace Core
{
	class Engine;
	struct Event;

	struct InputDevice
	{
		explicit InputDevice(Memory::IMemoryBlock& memory, const char* name, Input::DeviceType::Enum type)
			: name(name), id(-1), scheme(nullptr), type(type)// #NewAlloc
		{ }

		explicit InputDevice(Memory::IMemoryBlock& memory, std::int32_t deviceId);

		virtual ~InputDevice();

		Utils::String			name;
		Input::ControlScheme*	scheme;
		Input::DeviceType::Enum	type;
		std::int32_t 			id;
	};

	class InputManager : public IManager
	{
		MANAGER(InputManager);

		InputDevice				Mouse;
		InputDevice				Keyboard;

		Input::ControlScheme*	_currentScheme;

		std::map<std::int32_t, InputDevice*> _controllerIdMap;

		void onDeviceConnected		(const Input::ConnectionEvent& event);
		void onDeviceDisconnected	(const Input::ConnectionEvent& event);
		void onKey					(InputDevice* device, std::int32_t scancode, std::int32_t state, Core::GameTime& time);
		void onAxis					(InputDevice* device, std::int32_t axis, std::int32_t x, std::int32_t y, Core::GameTime& time);

		auto getDeviceForEvent		(const Input::InputEvent& event) -> InputDevice*;
		auto getDeviceForId			(std::int32_t id) -> InputDevice*;
		auto getSchemeForDevice		(InputDevice* device) -> Input::ControlScheme*;

	public:
		explicit InputManager(Core::Engine& engine, Memory::IMemoryBlock& memory)
			: IManager(engine, memory), _currentScheme(nullptr),
			  Mouse   (_memory, "Mouse",    Input::DeviceType::MOUSE),
			  Keyboard(_memory, "Keyboard", Input::DeviceType::KEYBOARD)
		{ }

		virtual ~InputManager();

		inline void setScheme(Input::ControlScheme& scheme)
		{ _currentScheme = &scheme; }

		inline auto getCurrentScheme() const -> Input::ControlScheme*
		{ return _currentScheme; }

		void handleInputEvent(const Core::Event& event, Core::GameTime& gameTime);
	};
};

#endif //YAGE_INPUTMANAGER_H
