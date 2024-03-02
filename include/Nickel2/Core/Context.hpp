#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <mutex>
#include <unordered_map>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <glfw/glfw3native.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "Window.hpp"
#include "Logger.hpp"

#include "../Renderer/Renderer.hpp"

namespace Nickel2 {
    class Logger;

    class Context {
        private:
            std::unordered_map<uint64_t, Window*> windows;
            Window* currentWindow;
            
            uint64_t genUniqueWindowID();

        public:
            std::string vendorName, rendererName, glVersion, glslVersion;
            int32_t extensionCount, maxTessLevel;
            uint32_t displayRefreshRate;
            glm::ivec2 displaySize;
            ALCcontext* alcContext;
            std::mutex logMutex;
            Logger* logger;

            Context(uint32_t logLevel = NICKEL2_INFO);
            ~Context();

            uint64_t registerWindow(Window* window);
            void makeCurrent();
            void makeWindowCurrent(uint64_t id);
            Window* getCurentWindow();
            void pollEvents();
            void removeWindow(uint64_t id);
            void destroy();
    };

    extern Context* context;
    Context* getContext();
}