#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "Windows.h"
#endif

#include "logger.hpp"

namespace nickel2 {
    void setTextAttribute(uint32_t color) {
        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
        #elif defined(__linux__)
            else if (color == NICKEL2_WHITE) {
                std::cout << "\e[97m";
            } else if (color == NICKEL2_BRIGHT_CYAN) {
                std::cout << "\e[96m";
            } else if (color == NICKEL2_BRIGHT_MAGENTA) {
                std::cout << "\e[95m";
            } else if (color == NICKEL2_BRIGHT_BLUE) {
                std::cout << "\e[94m";
            } else if (color == NICKEL2_BRIGHT_YELLOW) {
                std::cout << "\e[93m";
            } else if (color == NICKEL2_BRIGHT_GREEN) {
                std::cout << "\e[92m";
            } else if (color == NICKEL2_BRIGHT_RED) {
                std::cout << "\e[91m";
            } else if (color == NICKEL2_BRIGHT_BLACK) {
                std::cout << "\e[90m";
            } else if (color == NICKEL2_DARK_WHITE) {
                std::cout << "\e[37m";
            } else if (color == NICKEL2_DARK_CYAN) {
                std::cout << "\e[36m";
            } else if (color == NICKEL2_DARK_MAGENTA) {
                std::cout << "\e[35m";
            } else if (color == NICKEL2_DARK_BLUE) {
                std::cout << "\e[34m";
            } else if (color == NICKEL2_DARK_YELLOW) {
                std::cout << "\e[33m";
            } else if (color == NICKEL2_DARK_GREEN) {
                std::cout << "\e[32m";
            } else if (color == NICKEL2_DARK_RED) {
                std::cout << "\e[31m";
            } else if (color == NICKEL2_BLACK) {
                std::cout << "\e[30m";
            } else {

            }
        #endif
    }

    Logger::Logger() {
        level = NICKEL2_INFO;
    }

    Logger::~Logger() {

    }

    void Logger::setLevel(uint32_t level) {
        this->level = level;
    }

    void Logger::log(uint32_t type, const char* message) {
        if (level <= type) {
            const char* typeNames[] = {"info", "warning", "error", "fatal error"};
            uint32_t typeColors[] = {NICKEL2_BRIGHT_GREEN, NICKEL2_BRIGHT_MAGENTA, NICKEL2_BRIGHT_RED, NICKEL2_DARK_RED};
            uint32_t messageColors[] = {NICKEL2_DARK_WHITE, NICKEL2_BRIGHT_MAGENTA, NICKEL2_BRIGHT_RED, NICKEL2_DARK_RED};

            setTextAttribute(NICKEL2_WHITE);
            std::cout << "nickel2: ";
            setTextAttribute(typeColors[type]);
            std::cout << typeNames[type] << ": ";
            setTextAttribute(messageColors[type]);
            std::cout << message << std::endl;
            setTextAttribute(NICKEL2_WHITE);
        }
    }

    void Logger::destroy() {

    }
}