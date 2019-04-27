//
// Created by bentoo on 17.05.18.
//

#include "Platform/Logger.h"
#include "Platform/FileLoggerOutput.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace Core
{
    class SpdLoggerOutput : public ILoggerOutput
    {
        std::shared_ptr<spdlog::logger> Spdlogger;

    public:
        explicit SpdLoggerOutput(std::shared_ptr<spdlog::logger> logger)
            : Spdlogger(logger)
        { }

        virtual void PrintMessage(LogLevel::TYPE level, const char* message) override
        {
            Spdlogger->log((spdlog::level::level_enum)((int)level), message);
        }
    };

    FileLoggerOutput::FileLoggerOutput(const char* file_path)
        : Filelogger(spdlog::rotating_logger_mt("log", file_path, Memory::MB(100), 1))
    { }

    void FileLoggerOutput::PrintMessage(Core::LogLevel::TYPE level, const char* message)
    {
        Filelogger->log((spdlog::level::level_enum)((int)level), message);
    }

    // I hate this language so much...
    char Logger::_raw_memory[];

	Logger& Logger::get()
	{
		static Logger instance;
		return instance;
	}

    Logger::Logger()
    : _log_level(LogLevel::debug)
    , _allocator(_raw_memory, _raw_size)
    , _memory   (_allocator, "LogMemory")
    , _outputs  ()// #NewAlloc
    {
        createOutput<SpdLoggerOutput>(spdlog::stdout_color_mt("terminal"));
    }

    void Logger::destroy()
    {
        for (auto* output : _outputs)
        {
            Memory::Delete(_memory, output);
        }
    }
}
