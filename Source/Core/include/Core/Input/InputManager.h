//
// Created by Bentoo on 2017-02-10.
//

#ifndef YAGE_INPUTMANAGER_H
#define YAGE_INPUTMANAGER_H

#include <Utils/MemoryBlock.h>

namespace Core
{
	class Engine;
	class ControlScheme;

	class InputManager
	{
		Memory::IMemoryBlock& _memory;
		Core::Engine& _engine;

		const ControlScheme* _currentScheme;

	public:
		explicit InputManager(Core::Engine& engine, Memory::IMemoryBlock& memory)
			: _engine(engine), _memory(memory), _currentScheme(nullptr)
		{ }

		inline void setScheme(const ControlScheme& scheme)
		{
			_currentScheme = &scheme;
		}
	};
};

#endif //YAGE_INPUTMANAGER_H
