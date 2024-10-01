#pragma once

#include <iostream>
#include <ctime>
#include <vector>
#include <iomanip>
#include <sstream>
#include <string>
#include <mutex>

#include "Platform.hpp"

#define NK_BLACK 0
#define NK_DARK_RED 4
#define NK_DARK_GREEN 2
#define NK_DARK_YELLOW 6
#define NK_DARK_BLUE 1
#define NK_DARK_MAGENTA 5
#define NK_DARK_CYAN 3
#define NK_DARK_WHITE 7
#define NK_BRIGHT_BLACK 8
#define NK_BRIGHT_RED 12
#define NK_BRIGHT_GREEN 10
#define NK_BRIGHT_YELLOW 14
#define NK_BRIGHT_BLUE 9
#define NK_BRIGHT_MAGENTA 13
#define NK_BRIGHT_CYAN 11
#define NK_WHITE 15

namespace Nickel2 {
    std::string strftime(const char* format, const std::tm* time);
    void SetTextAttribute(uint32_t color);

    class Logger {
        public:
            enum class Level : uint8_t {
                Trace = 0, Info, Warn, Error, Fatal
            };

            static const char* LevelToString(Level level) {
                switch (level) {
                    case Level::Trace: return "Trace";
                    case Level::Info: return "Info";
                    case Level::Warn: return "Warning";
                    case Level::Error: return "Error";
                    case Level::Fatal: return "Fatal Error";
                } return "";
            }

        private:
            static Level level;
            static uint32_t limit;
            static std::vector<std::string> logs;
            static std::mutex logMutex;

        public:
            static void Initialize();
            static void Terminate();

            static void Log(Level level, const char* prefix, const char* message);
            static void Log(Level level, const char* message);
            static std::vector<std::string>& GetLogs();

            static void SetLevel(Level level);
            static Level GetLevel();

            static void SetLimit(uint32_t limit);
            static uint32_t GetLimit();
    };
}

#define NK_CORE_TRACE_TAG(prefix, message) Nickel2::Logger::Log(Nickel2::Logger::Level::Trace, prefix, message)
#define NK_CORE_INFO_TAG(prefix, message) Nickel2::Logger::Log(Nickel2::Logger::Level::Info, prefix, message)
#define NK_CORE_WARN_TAG(prefix, message) Nickel2::Logger::Log(Nickel2::Logger::Level::Warn, prefix, message)
#define NK_CORE_ERROR_TAG(prefix, message) Nickel2::Logger::Log(Nickel2::Logger::Level::Error, prefix, message)
#define NK_CORE_FATAL_ERROR_TAG(prefix, message) Nickel2::Logger::Log(Nickel2::Logger::Level::Fatal, prefix, message)

#define NK_CORE_TRACE(message) Nickel2::Logger::Log(Nickel2::Logger::Level::Trace, message)
#define NK_CORE_INFO(message) Nickel2::Logger::Log(Nickel2::Logger::Level::Info, message)
#define NK_CORE_WARN(message) Nickel2::Logger::Log(Nickel2::Logger::Level::Warn, message)
#define NK_CORE_ERROR(message) Nickel2::Logger::Log(Nickel2::Logger::Level::Error, message)
#define NK_CORE_FATAL_ERROR(message) Nickel2::Logger::Log(Nickel2::Logger::Level::Fatal, message)