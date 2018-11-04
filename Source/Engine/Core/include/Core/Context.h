//
// Created by mrjaqbq on 03.05.16.
//

#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <Utils/BorrowedPtr.h>

namespace Core
{
	class Logger;

	class Config;

	struct Context
	{
	private:
		Context() = delete;

	public:
 		Utils::borrowed_ptr<Core::Config> Config;
		Utils::borrowed_ptr<Core::Logger> Logger;

		// ToDo: this shall be looked upon
		Context(Utils::borrowed_ptr<Core::Config> conf, Utils::borrowed_ptr<Core::Logger> log) :
			Config(std::move(conf)),
			Logger(std::move(log))
		{ }

		//Context(Context&& ctx) : Config(ctx.Config.release()), Logger(ctx.Logger.release())
		//{ }

		// Context(const Context& ctx) : Config(ctx.Config.getRaw()), Logger(ctx.Logger.getRaw())
		// { }

		auto operator=(Context&& ctx) -> Context&;

		auto operator=(const Context& ctx) -> Context&;
	};
}

#endif //GAME_CONTEXT_H
