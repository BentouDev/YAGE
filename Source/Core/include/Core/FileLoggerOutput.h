//
// Created by bentoo on 17.05.18.
//

#ifndef YAGE_FILELOGGEROUTPUT_H
#define YAGE_FILELOGGEROUTPUT_H

#include "Logger.h"

namespace spdlog
{
    class logger;
}

namespace Core
{
    class FileLoggerOutput : public ILoggerOutput
    {
        std::shared_ptr<spdlog::logger> Filelogger;

    public:
        explicit FileLoggerOutput(const char* file_path);

        virtual void PrintMessage(LogLevel::TYPE level, const char* message) override;
    };
}

#endif //YAGE_FILELOGGEROUTPUT_H
