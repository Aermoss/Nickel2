#pragma once

#include <iostream>

#define NICKEL2_INFO 0
#define NICKEL2_WARNING 1
#define NICKEL2_ERROR 2
#define NICKEL2_FATAL_ERROR 3
#define NICKEL2_NONE 4

#define NICKEL2_BLACK 0
#define NICKEL2_DARK_RED 4
#define NICKEL2_DARK_GREEN 2
#define NICKEL2_DARK_YELLOW 6
#define NICKEL2_DARK_BLUE 1
#define NICKEL2_DARK_MAGENTA 5
#define NICKEL2_DARK_CYAN 3
#define NICKEL2_DARK_WHITE 7
#define NICKEL2_BRIGHT_BLACK 8
#define NICKEL2_BRIGHT_RED 12
#define NICKEL2_BRIGHT_GREEN 10
#define NICKEL2_BRIGHT_YELLOW 14
#define NICKEL2_BRIGHT_BLUE 9
#define NICKEL2_BRIGHT_MAGENTA 13
#define NICKEL2_BRIGHT_CYAN 11
#define NICKEL2_WHITE 15

namespace nickel2 {
    void setTextAttribute(uint32_t color);

    class Logger {
        private:
            uint32_t level;

        public:
            Logger();
            ~Logger();

            void setLevel(uint32_t level);
            void log(uint32_t type, const char* message);
            void destroy();
    };
}