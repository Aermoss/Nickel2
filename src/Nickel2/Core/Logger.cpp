#include <Nickel2/nkpch.hpp>
#include <Nickel2/Core/Logger.hpp>

namespace Nickel2 {
    std::string strftime(const char* format, const std::tm* time) {
        std::ostringstream os;
        os << std::put_time(time, format);
        return os.str();
    }

    void SetTextAttribute(uint32_t color) {
        #ifdef NK_PLATFORM_WINDOWS
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
        #endif
    }

    std::vector<std::string> Logger::logs = {};
    std::mutex Logger::logMutex = {};
    Logger::Level Logger::level = Logger::Level::Trace;
    uint32_t Logger::limit = 512;

    void Logger::Initialize() {
        Logger::level = Logger::Level::Trace;
        Logger::limit = 512;
    }

    void Logger::Terminate() {

    }

    void Logger::Log(Level level, const char* prefix, const char* message) {
        if (Logger::level <= level) {
            std::time_t now;
            std::time(&now);
            std::tm* tmp = localtime(&now);
            if (logs.size() >= Logger::limit) logs.erase(logs.begin());
            uint32_t levelColors[] = { NK_BRIGHT_YELLOW, NK_BRIGHT_GREEN, NK_BRIGHT_MAGENTA, NK_BRIGHT_RED, NK_DARK_RED };
            uint32_t prefixColors[] = { NK_BRIGHT_YELLOW, NK_BRIGHT_YELLOW, NK_BRIGHT_MAGENTA, NK_BRIGHT_RED, NK_DARK_RED };
            uint32_t messageColors[] = { NK_DARK_WHITE, NK_DARK_WHITE, NK_BRIGHT_MAGENTA, NK_BRIGHT_RED, NK_DARK_RED };

            std::lock_guard<std::mutex> lock(Logger::logMutex);
            logs.push_back(strftime("[%m/%d/%Y-%H:%M]: ", tmp) + "Nickel2: " + LevelToString(level) + ": " + std::string(prefix) + ":" + std::string(message));

            SetTextAttribute(NK_WHITE);
            std::cout << "Nickel2: ";
            SetTextAttribute(levelColors[static_cast<uint32_t>(level)]);
            std::cout << LevelToString(level) << ": ";
            SetTextAttribute(prefixColors[static_cast<uint32_t>(level)]);
            std::cout << prefix << ": ";
            SetTextAttribute(messageColors[static_cast<uint32_t>(level)]);
            std::cout << message << std::endl;
            SetTextAttribute(NK_WHITE);
        }
    }

    void Logger::Log(Level level, const char* message) {
        if (Logger::level <= level) {
            std::time_t now;
            std::time(&now);
            std::tm* tmp = localtime(&now);
            if (logs.size() >= Logger::limit) logs.erase(logs.begin());
            uint32_t typeColors[] = { NK_BRIGHT_YELLOW, NK_BRIGHT_GREEN, NK_BRIGHT_MAGENTA, NK_BRIGHT_RED, NK_DARK_RED };
            uint32_t messageColors[] = { NK_DARK_WHITE, NK_DARK_WHITE, NK_BRIGHT_MAGENTA, NK_BRIGHT_RED, NK_DARK_RED };

            std::lock_guard<std::mutex> lock(Logger::logMutex);
            logs.push_back(strftime("[%m/%d/%Y-%H:%M]: ", tmp) + "Nickel2: " + LevelToString(level) + ": " + std::string(message));

            SetTextAttribute(NK_WHITE);
            std::cout << "Nickel2: ";
            SetTextAttribute(typeColors[static_cast<uint32_t>(level)]);
            std::cout << LevelToString(level) << ": ";
            SetTextAttribute(messageColors[static_cast<uint32_t>(level)]);
            std::cout << message << std::endl;
            SetTextAttribute(NK_WHITE);
        }
    }

    std::vector<std::string>& Logger::GetLogs() {
        return Logger::logs;
    }

    void Logger::SetLevel(Logger::Level level) {
        Logger::level = level;
    }

    Logger::Level Logger::GetLevel() {
        return Logger::level;
    }

    void Logger::SetLimit(uint32_t limit) {
        Logger::limit = limit;
    }

    uint32_t Logger::GetLimit() {
        return Logger::limit;
    }
}