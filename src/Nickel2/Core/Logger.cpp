#include <Nickel2/nkpch.hpp>
#include <Nickel2/Core/Logger.hpp>

namespace Nickel2 {
    void setTextAttribute(uint32_t color) {
        #ifdef NICKEL2_PLATFORM_WINDOWS
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
        #endif
    }

    std::string strftime(const char* format, const std::tm* time) {
        std::ostringstream os;
        os << std::put_time(time, format);
        return os.str();
    }

    Logger::Logger() {
        level = NICKEL2_INFO, limit = 1024;
    }

    Logger::~Logger() {

    }

    void Logger::log(uint32_t type, const char* message) {
        if (level <= type) {
            std::time_t now;
            std::time(&now);
            std::tm* tmp = localtime(&now);
            if (logs.size() >= limit) logs.erase(logs.begin());
            const char* typeNames[] = {"info", "warning", "error", "fatal error"};
            uint32_t typeColors[] = {NICKEL2_BRIGHT_GREEN, NICKEL2_BRIGHT_MAGENTA, NICKEL2_BRIGHT_RED, NICKEL2_DARK_RED};
            uint32_t messageColors[] = {NICKEL2_DARK_WHITE, NICKEL2_BRIGHT_MAGENTA, NICKEL2_BRIGHT_RED, NICKEL2_DARK_RED};

            std::lock_guard<std::mutex> lock(Nickel2::getContext()->logMutex);
            logs.push_back(strftime("[%m/%d/%Y-%H:%M]: ", tmp) + "nickel2: " + std::string(typeNames[type]) + ": " + std::string(message));

            setTextAttribute(NICKEL2_WHITE);
            std::cout << "nickel2: ";
            setTextAttribute(typeColors[type]);
            std::cout << typeNames[type] << ": ";
            setTextAttribute(messageColors[type]);
            std::cout << message << std::endl;
            setTextAttribute(NICKEL2_WHITE);
        }
    }

    std::vector <std::string>& Logger::getLogs() {
        return logs;
    }

    void Logger::setLevel(uint32_t level) {
        this->level = level;
    }

    uint32_t Logger::getLevel() {
        return level;
    }

    void Logger::setLimit(uint32_t limit) {
        this->limit = limit;
    }

    uint32_t Logger::getLimit() {
        return limit;
    }

    void Logger::destroy() {

    }
}