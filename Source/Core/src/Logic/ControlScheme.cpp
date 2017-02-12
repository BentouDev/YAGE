//
// Created by Bentoo on 2017-02-11.
//

#include "Core/Input/ControlScheme.h"
#include "Core/Platform.h"
#include "Core/GameTime.h"
#include "Core/Logger.h"

namespace Logic
{
	ControlScheme::ControlScheme(Memory::IMemoryBlock& memory)
		: _memory(memory),
		  _controlActions(_memory),
		  _buttonStateDatas(_memory),
		  _buttonStateEnums(_memory),
		  _boundScancodes(_memory)
	{

	}

	ControlAction* ControlScheme::bindAction(ControlAction* action, std::uint32_t scancode)
	{
		return bindAction(action->actionName, scancode);
	}

	ControlAction* ControlScheme::bindAction(const char* name, std::uint32_t scancode)
	{
		ControlAction* result = nullptr;

		auto itr = _nameMap.find(name);
		if(itr != _nameMap.end())
		{
			auto	index	= itr->second;
					result	= _controlActions[index];

			_scancodeMap.erase(result->boundScancode);

			result->boundScancode	= scancode;
			_boundScancodes[index]	= scancode;
			_scancodeMap[scancode]	= index;
		}
		else
		{
			auto*	action	= YAGE_CREATE_NEW(_memory, ControlAction)(this);
			auto	index	= _controlActions.size();

			action->actionName		= name;
			action->boundScancode	= scancode;

			_controlActions.add(action);
			_boundScancodes.add(scancode);
			_scancodeMap[scancode] = index;

			result = action;
		}

		return result;
	}

	void ControlScheme::updateButtonByIndex(std::uint32_t index, std::uint32_t state, const Core::GameTime& time)
	{
		ControlAction*		action		= _controlActions[index];
		ButtonStateData&	stateData	= _buttonStateDatas[index];
		ButtonStateEnum&	stateEnum	= _buttonStateEnums[index];

		bool button		= state != GLFW_RELEASE;
		bool clean		= stateEnum == None;
		bool changed	= stateData.Value != state;
		bool buffered	= stateData.Frames < FramesInBuffer;

		stateData.Value = state;
		// This one was created before, has a bug when on release it sends pressed once again
		// result.State = (button && changed) || buffered ? ButtonState.Pressed : ButtonState.None;

		// This one was created after, to prevent from above bug
		action->buttonState = (!button && ((changed && clean) || buffered)) ? Pressed : None;

		if (!changed)
		{
			stateData.Elapsed += time.DeltaTime;

			if (stateData.Elapsed > FrameTime && buffered)
			{
				stateData.Frames++;
				stateData.Elapsed = 0;
			}
		}
		else
		{
			stateData.Elapsed = 0;
			stateData.Frames = 0;
		}

		if (changed || stateData.Elapsed < RapidTimeTreshold)
		{
			stateData.RapidPotential++;
			action->buttonState = stateData.RapidPotential > RapidFrameRequirement ? Rapid : action->buttonState;
		}
		else
		{
			stateData.RapidPotential = 0;
		}

		action->buttonState = button && stateData.Elapsed > HoldTimeRequirement ? Hold : action->buttonState;

		stateEnum = action->buttonState;
	}

	void ControlScheme::updateButtonByScancode(std::uint32_t scancode, std::uint32_t state, const Core::GameTime& time)
	{
		auto itr = _scancodeMap.find(scancode);
		if(itr == _scancodeMap.end())
			return;

		auto index = itr->second;
		updateButtonByIndex(index, state, time);
	}

	ControlAction* ControlScheme::getAction(const char *name)
	{
		auto itr = _nameMap.find(name);
		if(itr == _nameMap.end())
			return nullptr;

		return _controlActions[itr->second];
	}
}
