//
// Created by mrjaqbq on 31.03.16.
//

#ifndef GAME_LOGGER_H
#define GAME_LOGGER_H

#include <spdlog/spdlog.h>
#include <Utils/Handle.h>

namespace Core
{
	class Engine;

	class Config;

	class Logger
	{
		friend class Engine;

	public:
		static Logger& get()
		{
			static Logger instance;
			return instance;
		}

		std::shared_ptr<spdlog::logger> Default;

	private:
		static constexpr char const* defaultSinkName = "console";
		Logger() : Default(spdlog::stdout_logger_mt(defaultSinkName, true)) { };

	public:
		virtual ~Logger() { destroy(); }

		Logger(Logger const&) = delete;
		Logger(Logger&&) = delete;
		void operator=(Logger const&) = delete;
		void operator=(Logger&&) = delete;

		template <typename... Args> static void log(spdlog::level::level_enum lvl, const char* fmt, const Args&... args)
		{ get().Default->log(lvl, fmt, args...); }

		template <typename... Args> static void log(spdlog::level::level_enum lvl, const char* msg)
		{ get().Default->log(lvl, msg); }

		template <typename... Args> static void trace(const char* fmt, const Args&... args)
		{ get().Default->trace(fmt, args...); }

		template <typename... Args> static void debug(const char* fmt, const Args&... args)
		{ get().Default->trace(fmt, args...); }

		template <typename... Args> static void info(const char* fmt, const Args&... args)
		{ get().Default->info(fmt, args...); }

		template <typename... Args> static void warn(const char* fmt, const Args&... args)
		{ get().Default->warn(fmt, args...); }

		template <typename... Args> static void error(const char* fmt, const Args&... args)
		{ get().Default->error(fmt, args...); }

		template <typename... Args> static void critical(const char* fmt, const Args&... args)
		{ get().Default->critical(fmt, args...); }

		spdlog::logger* operator->()
		{
			return Default.get();
		}

		inline void destroy()
		{
			spdlog::drop(defaultSinkName);
		}
	};
}

#endif //GAME_LOGGER_H
