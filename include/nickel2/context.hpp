#pragma once

#include <stdint.h>
#include <iostream>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "window.hpp"
#include "logger.hpp"
#include "renderer.hpp"

namespace nickel2 {
    class Context {
        private:
            std::unordered_map <uint32_t, Window*> windows;
            bool running;
            
            uint32_t genUniqueWindowID();

        public:
            std::string vendorName, rendererName, glVersion, glslVersion;
            uint32_t displayRefreshRate;
            glm::ivec2 displaySize;
            int32_t extensionCount;
            ALCcontext* alcContext;
            Logger* logger;

            Context(uint32_t logLevel = NICKEL2_INFO);
            ~Context();

            uint32_t registerWindow(Window* window);
            void makeCurrent();
            void pollEvents();
            void removeWindow(uint32_t id);
            void destroy();
    };

    extern Context* context;
    Context* getContext();
}