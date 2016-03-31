//
// Created by mrjaqbq on 31.03.16.
//

#ifndef GAME_LOGGER_H
#define GAME_LOGGER_H

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
    private:
        Logger() {};

    public:
        Logger(Logger const&) = delete;
        Logger(Logger&&) = delete;
        void operator=(Logger const&) = delete;
        void operator=(Logger&&) = delete;
    };
}

#endif //GAME_LOGGER_H
