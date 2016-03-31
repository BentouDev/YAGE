//
// Created by mrjaqbq on 31.03.16.
//

#ifndef GAME_LOGGER_H
#define GAME_LOGGER_H

#include <spdlog/spdlog.h>

namespace Core
{
	class Logger
	{
	public:
		static Logger& getInstance()
		{
			static Logger instance;
			return instance;
		}

		std::shared_ptr<spdlog::logger> Console;

	private:
		Logger() : Console(spdlog::stdout_logger_mt("console")) { };

	public:
		Logger(Logger const&) = delete;
		Logger(Logger&&) = delete;
		void operator=(Logger const&) = delete;
		void operator=(Logger&&) = delete;
	};
}

#endif //GAME_LOGGER_H
