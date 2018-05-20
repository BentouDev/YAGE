//
// Created by Bentoo on 2017-02-10.
//

#ifndef YAGE_CONTROLSCHEME_H
#define YAGE_CONTROLSCHEME_H

#include <map>

#include <Utils/MemoryBlock.h>
#include <Utils/List.h>
#include <Core/Input/DeviceType.h>

namespace Core
{
	struct GameTime;
	class InputManager;
}

namespace Input
{
	class ControlScheme;

	enum ButtonStateEnum : std::uint8_t
	{
		None,
		Pressed,
		Hold,
		Rapid
	};

	struct ButtonStateData
	{
		int		RapidPotential;
		int		Frames;
		float	Elapsed;
		int		Value;
	};

	class ControlAction
	{
		friend class ControlScheme;

	private:
		ControlScheme* _parentScheme;

		std::uint32_t	boundScancode;
		std::string		actionName;
		ButtonStateEnum	buttonState;

	public:
		explicit ControlAction(ControlScheme* parentScheme)
			: _parentScheme(parentScheme)
		{ }

		bool isPressed() const
		{ return buttonState == Pressed; }

		bool isHold() const
		{ return buttonState == Hold; }

		bool isRapid() const
		{ return buttonState == Rapid; }
	};

	class ControlScheme
	{
		friend class Core::InputManager;

	private:
		Memory::IMemoryBlock&			_memory;
		Utils::List<ControlAction*>		_controlActions;
		Utils::List<ButtonStateData>	_buttonStateDatas;

		std::map<std::uint32_t, std::uint32_t>	_deviceCodeMap[DeviceType::_COUNT];
		std::map<std::string,   std::uint32_t>	_nameMap;

		void updateButtonByScancode	(DeviceType::Enum device,
									 std::uint32_t scancode, std::uint32_t state,
									 const Core::GameTime& time);

		void updateAxisById			(DeviceType::Enum device,
									 std::int32_t axisId, std::int32_t x, std::int32_t y,
									 const Core::GameTime& time);

		void updateButtonByIndex	(std::uint32_t index, std::uint32_t state,
									 const Core::GameTime& time);

		// 1, 32
		int FramesInBuffer { 4 };

		// 1.0f / 60.0f, 1.0f
		float FrameTime { 0.01666667f };

		// 1.0f / 60.0f, 1.f
		float RapidTimeTreshold { 0.02f };

		// 1, 32
		int RapidFrameRequirement { 25 };

		// 1.0f / 60.0f, 1.f
		float HoldTimeRequirement { 0.05f };

	public:
		explicit ControlScheme(Memory::IMemoryBlock& memory);
		virtual ~ControlScheme();

		ControlAction* getAction(std::string name);

		ControlAction* bindAction(DeviceType::Enum device, std::string name, std::uint32_t scancode);
	};
}

#endif //YAGE_CONTROLSCHEME_H
