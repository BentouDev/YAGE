//
// Created by mrjaqbq on 31.03.16.
//

#ifndef GAME_LOGGER_H
#define GAME_LOGGER_H

#include <spdlog/spdlog.h>
#include "Handle.h"

namespace Core
{
	class Engine;

	class Config;

	class Logger
	{
		friend class Engine;

		Utils::borrowed_ptr<Config> config;

		auto setConfig(Utils::borrowed_ptr<Config> conf) -> void
		{
			config.reset(conf.release());
		}

	public:
	//	static Logger& get()
	//	{
	//		static Logger instance;
	//		return instance;
	//	}

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
