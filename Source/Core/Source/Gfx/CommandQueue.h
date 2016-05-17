//
// Created by mrjaqbq on 14.05.16.
//

#ifndef GAME_COMMANDQUEUE_H
#define GAME_COMMANDQUEUE_H

namespace Gfx
{
	template <typename T>
	class CommandQueue
	{
		using Key = T;

		Key* keys;
		void** data;

		template <typename U>
		void AddCommand(Key key);

		void Submit();
		void Sort();
	};
}

#endif //GAME_COMMANDQUEUE_H
