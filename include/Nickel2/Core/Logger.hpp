#pragma once

#include <iostream>
#include <ctime>
#include <vector>
#include <iomanip>
#include <sstream>
#include <string>

namespace Nickel2 {
    class Context;
    Context* getContext();
}

#include "Platform.hpp"
#include "Context.hpp"
#include "Constants.hpp"

#define NICKEL2_LOG(type, message) Nickel2::getContext()->logger->log(type, message)

namespace Nickel2 {
    void setTextAttribute(uint32_t color);
    std::string strftime(const char* format, const std::tm* time);

    class Logger {
        private:
            uint32_t level, limit;

        public:
            std::vector<std::string> logs;

            Logger();
            ~Logger();

            void log(uint32_t type, const char* message);
            std::vector<std::string>& getLogs();
            void setLevel(uint32_t level);
            uint32_t getLevel();
            void setLimit(uint32_t limit);
            uint32_t getLimit();
            void destroy();
    };
}