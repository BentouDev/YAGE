//
// Created by mrjaqbq on 14.05.16.
//

#include "CommandQueue.h"

namespace Gfx
{
	/*auto CommandQueue::CreateCommand() -> Command&
	{
//		Command* new_cmd = new (memory) Command();
//		memory++;
//		return *new_cmd;

		Command* cmd  = new Command();

		// cmd->function = _api->getDraw

		return *cmd;
	}

	void CommandQueue::SubmitCommand(Command& cmd)
	{
		_commands = &cmd;
	}

	void CommandQueue::Submit()
	{
		// bind matrices
		// bind rendertarget
		// clear rendertarget, if needed

		if(_commands == nullptr)
			return;

		const CommandFunction fn = *_commands->function;
		fn(*_commands);

		// issue call on each command
		// for(int i = 0; i < commandCount; i++)
		{
			// ptr* cmd = commands[i]
			// do
			// bind data
			// call command
			// cmd = next
			// while (has cmd)
		}

		// flush rendertarget
	}*/
}
