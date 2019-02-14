//
// Created by mrjaqbq on 31.03.16.
//

#ifndef YAGE_LOGGER_H
#define YAGE_LOGGER_H

#include <exception>
#include <Utils/String.h>
#include <Utils/FreeListAllocator.h>
#include <Utils/MemorySizes.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <spdlog/fmt/fmt.h>

#ifdef CREATE_NEW
#undef CREATE_NEW
#endif

#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef GetCurrentTime
#undef GetCurrentTime
#endif

namespace Core
{
    class Engine;

    class Config;

    namespace LogLevel
    {
        enum TYPE
        {
            trace    = 0,
            debug    = 1,
            info     = 2,
            warn     = 3,
            error    = 4,
            critical = 5,
            off      = 6
        };
    }

    class ILoggerOutput
    {
    public:
        virtual ~ILoggerOutput()
        { }

        virtual void PrintMessage(LogLevel::TYPE level, const char* message) = 0;
    };

    class Logger
    {
        friend class Engine;

        using TLoggerAllocator = Memory::FreeListAllocator;
        using TLoggerMemory    = Memory::MemoryBlock<TLoggerAllocator>;

        static constexpr size_t _raw_size = Memory::KB(10);
        static char _raw_memory[_raw_size];

    public:
        static Logger& get()
        {
            static Logger instance;
            return instance;
        }

        LogLevel::TYPE              _log_level;
        TLoggerMemory               _memory;
        TLoggerAllocator            _allocator;
        Utils::List<ILoggerOutput*> _outputs;

    private:
        Logger();

    public:
        virtual ~Logger()
        { destroy(); }

        Logger(Logger const&)         = delete;
        Logger(Logger&&)              = delete;
        void operator=(Logger const&) = delete;
        void operator=(Logger&&)      = delete;

        void destroy();

        template <typename T, typename ... Args>
        void createOutput(Args&&... args)
        {
            try
            {
                auto* _output = YAGE_CREATE_NEW(_memory, T)(std::forward<Args>(args)...);
                _outputs.add(_output);
            }
            catch (std::exception e)
            {
                YAGE_ASSERT(false, "Unable to create logger output:\n {0}}", e.what());
            }
        }

        static void setLogLevel(LogLevel::TYPE level)
        { get()._log_level = level; }

        template <typename... Args> static void log(LogLevel::TYPE lvl, const char* str_format, const Args&... args)
        {
            using TBuffer = fmt::memory_buffer;
            if (lvl < get()._log_level) return;

            TBuffer raw;
            fmt::format_to(raw, str_format, args...);

            for (auto* output : get()._outputs)
            {
                output->PrintMessage(lvl, raw.data());
            }
        }

        template <typename... Args> static void log(LogLevel::TYPE lvl, const char* msg)
        {
            if (lvl < get()._log_level) return;

            for (auto* output : get()._outputs)
            {
                output->PrintMessage(lvl, msg);
            }
        }

        template <typename... Args> static void trace(const char* fmt, const Args&... args)
        { log(LogLevel::trace, fmt, args...); }

        template <typename... Args> static void debug(const char* fmt, const Args&... args)
        { log(LogLevel::debug, fmt, args...); }

        template <typename... Args> static void info(const char* fmt, const Args&... args)
        { log(LogLevel::info, fmt, args...); }

        template <typename... Args> static void warn(const char* fmt, const Args&... args)
        { log(LogLevel::warn, fmt, args...); }

        template <typename... Args> static void error(const char* fmt, const Args&... args)
        { log(LogLevel::error, fmt, args...); }

        template <typename... Args> static void critical(const char* fmt, const Args&... args)
        { log(LogLevel::critical, fmt, args...); }
    };
}

#endif //YAGE_LOGGER_H
