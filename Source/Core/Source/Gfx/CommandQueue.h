//
// Created by mrjaqbq on 14.05.16.
//

#ifndef GAME_COMMANDQUEUE_H
#define GAME_COMMANDQUEUE_H

#include <cstdint>

namespace Gfx
{
	class Command;

	typedef void (*CommandFunction)(Command& cmd);

	class Command
	{

	public:
		uint32_t count;
		uint32_t index;

		CommandFunction* function;

		auto setBuffer() -> void;

		auto setIndexBuffer() -> void;

		auto setCulling() -> void;

		auto setAlphaBlending() -> void;
	};

	class CommandQueue
	{
		Command* _commands;

	public:
		auto CreateCommand() -> Command&;

		auto SubmitCommand(Command& cmd) -> void;

		auto CleanUp() -> void;

		void Submit();
	};
}

#endif //GAME_COMMANDQUEUE_H
