#include "context.hpp"

namespace nickel2 {
    Context* context = nullptr;

    Context* getContext() {
        if (context == nullptr) {
            std::cout << "error: no context" << std::endl;
        } return context;
    }

    Context::Context() {
        running = false;

        if (context == nullptr) {
            context = this;
        } else {
            std::cout << "error: more than one context" << std::endl;
        }

        glfwInit();
    }

    Context::~Context() {

    }

    uint32_t Context::getUniqueWindowID() {
        uint32_t id = 0;
        while (windows.find(id) != windows.end()) id++;
        return id;
    }

    uint32_t Context::registerWindow(Window* window) {
        uint32_t id = getUniqueWindowID();
        windows[id] = window;
        return id;
    }

    void Context::removeWindow(uint32_t id) {
        windows.erase(id);
    }

    void Context::destroy() {
        if (windows.size() != 0) {
            std::cout << "warning: active windows" << std::endl;
        }

        context = nullptr;
        glfwTerminate();
    }
}