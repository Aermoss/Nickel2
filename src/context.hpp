#pragma once

#include <stdint.h>
#include <iostream>
#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.hpp"
#include "logger.hpp"

namespace nickel2 {
    class Window;
    
    class Context {
        private:
            std::map <uint32_t, Window*> windows;
            bool running;

        public:
            std::string vendorName, rendererName, glVersion, glslVersion, glExtensions;
            uint32_t displayRefreshRate;
            glm::ivec2 displaySize;
            Logger* logger;

            Context(uint32_t logLevel = NICKEL2_INFO);
            ~Context();

            uint32_t getUniqueWindowID();
            uint32_t registerWindow(Window* window);
            void removeWindow(uint32_t id);
            void destroy();
    };

    extern Context* context;
    Context* getContext();
}