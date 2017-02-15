//
// Created by Bentoo on 2017-02-10.
//

#ifndef YAGE_INPUTMANAGER_H
#define YAGE_INPUTMANAGER_H

#include <Utils/MemoryBlock.h>
#include <map>
#include <Core/GameTime.h>

struct _SDL_GameController;
typedef struct _SDL_GameController SDL_GameController;

namespace Input
{
	class ControlScheme;
	union InputEvent;
}

namespace Core
{
	class Engine;
	struct Event;

	struct InputDevice
	{
		explicit InputDevice(const char* name)
			: _name(name), _hController(nullptr), _scheme(nullptr)
		{ }

		explicit InputDevice(SDL_GameController* controller);

		virtual ~InputDevice();

		const char* 			_name;
		SDL_GameController*		_hController;
		Input::ControlScheme*	_scheme;
		std::int32_t 			_id;
	};

	class InputManager
	{
		InputDevice				MouseAndKeyboard;

		Memory::IMemoryBlock&	_memory;
		Core::Engine&			_engine;

		Input::ControlScheme*	_currentScheme;

		std::map<std::int32_t, InputDevice*> _controllerIdMap;

		void onDeviceConnected		(InputDevice* device);
		void onDeviceDisconnected	(InputDevice* device);
		void onKey					(InputDevice* device, std::int32_t scancode, std::int32_t state, Core::GameTime& time);
		void onAxis					(InputDevice* device, std::int32_t axis, std::int32_t x, std::int32_t y, Core::GameTime& time);

		auto getDeviceForId			(std::int32_t id) -> InputDevice*;
		auto getSchemeForDevice		(InputDevice* device) -> Input::ControlScheme*;

	public:
		explicit InputManager(Core::Engine& engine, Memory::IMemoryBlock& memory)
			: MouseAndKeyboard("MouseAndKeyboard"),
			  _engine(engine), _memory(memory), _currentScheme(nullptr)
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
