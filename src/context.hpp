#pragma once

#include <stdint.h>
#include <iostream>
#include <map>

#include <GLFW/glfw3.h>

namespace nickel2 {
    class Window;
    
    class Context {
        private:
            std::map <uint32_t, Window*> windows;
            bool running;

        public:
            Context();
            ~Context();

            uint32_t getUniqueWindowID();
            uint32_t registerWindow(Window* window);
            void removeWindow(uint32_t id);
            void destroy();
    };

    extern Context* context;
    Context* getContext();
}