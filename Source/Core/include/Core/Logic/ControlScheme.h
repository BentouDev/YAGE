//
// Created by Bentoo on 2017-02-10.
//

#ifndef YAGE_CONTROLSCHEME_H
#define YAGE_CONTROLSCHEME_H

#include <Utils/MemoryBlock.h>
#include <Utils/List.h>
#include <map>
#include <Utils/Range.h>

namespace Core
{
	struct GameTime;
}

namespace Logic
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
		const char*		actionName;
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
		friend class InputManager;

	private:
		Memory::IMemoryBlock&			_memory;
		Utils::List<ControlAction*>		_controlActions;
		Utils::List<ButtonStateData>	_buttonStateDatas;
		Utils::List<ButtonStateEnum>	_buttonStateEnums;
		Utils::List<std::uint32_t>		_boundScancodes;

		std::map<std::string,   std::uint32_t>	_nameMap;
		std::map<std::uint32_t, std::uint32_t>	_scancodeMap;

		void updateButtonByIndex	(std::uint32_t index,	 std::uint32_t state, const Core::GameTime& time);
		void updateButtonByScancode	(std::uint32_t scancode, std::uint32_t state, const Core::GameTime& time);

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

		ControlAction* getAction(const char* name);
		ControlAction* bindAction(ControlAction* action, std::uint32_t scancode);
		ControlAction* bindAction(const char* name, std::uint32_t scancode);
	};
}

#endif //YAGE_CONTROLSCHEME_H
