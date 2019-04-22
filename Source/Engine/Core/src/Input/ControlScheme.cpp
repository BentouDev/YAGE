//
// Created by Bentoo on 2017-02-11.
//

#include "Core/Input/ControlScheme.h"
#include "Core/Platform.h"
#include "Core/GameTime.h"
#include "Core/Logger.h"

namespace Input
{
	ControlScheme::ControlScheme(Memory::IMemoryBlock& memory)
		: _memory(memory),
		  _controlActions  (), // #NewAlloc
		  _buttonStateDatas()
	{ }

	ControlScheme::~ControlScheme()
	{
		for(auto action : _controlActions)
		{
			Memory::Delete(_memory, action);
		}
	}

	ControlAction* ControlScheme::bindAction(DeviceType::Enum device, std::string name, std::uint32_t scancode)
	{
		auto& codeMap = _deviceCodeMap[device];
		ControlAction* result = nullptr;

		auto itr = _nameMap.find(name);
		if(itr != _nameMap.end())
		{
			auto index  = itr->second;
				 result = _controlActions[index];

			codeMap.erase(result->boundScancode);

			result->boundScancode = scancode;
			codeMap[scancode]     = index;
		}
		else
		{
			auto*	action	= YAGE_CREATE_NEW(_memory, ControlAction)(this);
			auto	index	= _controlActions.size();

			action->actionName		= name;
			action->boundScancode	= scancode;

			_controlActions.push_back(action);
			_buttonStateDatas.emplace_back();
			_nameMap    [name]     = index;
			codeMap     [scancode] = index;

			result = action;
		}

		return result;
	}

	void ControlScheme::updateButtonByIndex(std::uint32_t index, std::uint32_t state, const Core::GameTime& time)
	{
		ControlAction*		action		= _controlActions[index];
		ButtonStateData&	stateData	= _buttonStateDatas[index];

		bool button		= state != GLFW_RELEASE;
		bool clean		= action->buttonState == None;
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
	}

	void ControlScheme::updateButtonByScancode(DeviceType::Enum device, std::uint32_t scancode, std::uint32_t state, const Core::GameTime& time)
	{
		auto& codeMap = _deviceCodeMap[device];
		auto itr = codeMap.find(scancode);
		if(itr == codeMap.end())
			return;

		auto index = itr->second;
		updateButtonByIndex(index, state, time);
	}

	ControlAction* ControlScheme::getAction(std::string name)
	{
		auto itr = _nameMap.find(name);
		if(itr == _nameMap.end())
			return nullptr;

		return _controlActions[itr->second];
	}
}
