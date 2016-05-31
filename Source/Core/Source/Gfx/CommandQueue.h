//
// Created by mrjaqbq on 14.05.16.
//

#ifndef GAME_COMMANDQUEUE_H
#define GAME_COMMANDQUEUE_H

namespace Gfx
{
	class Command
	{
	public:
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
	};
}

#endif //GAME_COMMANDQUEUE_H
