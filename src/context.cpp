#include <nickel2/context.hpp>

namespace nickel2 {
    Context* context = nullptr;

    Context* getContext() {
        if (context == nullptr) {
            std::cout << "error: no context" << std::endl;
        } return context;
    }

    uint32_t Context::genUniqueWindowID() {
        uint32_t id = 0;
        while (windows.find(id) != windows.end()) id++;
        return id;
    }

    Context::Context(uint32_t logLevel) {
        running = false;
        logger = new Logger();
        logger->setLevel(logLevel);

        if (context == nullptr) {
            context = this;
            context->logger->log(NICKEL2_INFO, "context created successuflly.");
        } else {
            context->logger->log(NICKEL2_ERROR, "failed to create context (there is already an existing context).");
        }

        if (!glfwInit()) {
            context->logger->log(NICKEL2_FATAL_ERROR, "failed to initialize glfw.");
        }

        GLFWmonitor* display = glfwGetPrimaryMonitor();
        const GLFWvidmode* videoMode = glfwGetVideoMode(display);
        displaySize = glm::ivec2(videoMode->width, videoMode->height);
        displayRefreshRate = videoMode->refreshRate;

        context->logger->log(NICKEL2_INFO, (
            std::string("found display device:\n") + \
            "               > size: " + std::to_string(displaySize.x) + " x " + std::to_string(displaySize.y) + "\n" + \
            "               > refresh rate: " + std::to_string(displayRefreshRate)).c_str()
        );

        if (!alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"))
            context->logger->log(NICKEL2_ERROR, "alc enumeration extension not available.");

        const char* defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
        ALCdevice* device = alcOpenDevice(defaultDeviceName);

        if (!device)
            context->logger->log(NICKEL2_ERROR, "unable to open default device.");

        context->logger->log(NICKEL2_INFO, ("found audio device: " + std::string(alcGetString(device, ALC_DEVICE_SPECIFIER)) + ".").c_str());
        alcContext = alcCreateContext(device, NULL);

        if (!alcMakeContextCurrent(alcContext)) {
            context->logger->log(NICKEL2_ERROR, "failed to make alc context current.");
        }
    }

    Context::~Context() {

    }

    uint32_t Context::registerWindow(Window* window) {
        uint32_t id = genUniqueWindowID();
        windows[id] = window;
        logger->log(NICKEL2_INFO, ("window named \"" + windows[id]->getTitle() + "\" was registered with id: " + std::to_string(id) + ".").c_str());
        return id;
    }

    void Context::makeCurrent() {
        alcMakeContextCurrent(alcContext);
        context = this;
    }

    void Context::pollEvents() {
        glfwPollEvents();
    }

    void Context::removeWindow(uint32_t id) {
        logger->log(NICKEL2_INFO, ("window named \"" + windows[id]->getTitle() + "\" was closed successfully, id: " + std::to_string(id) + ".").c_str());
        windows.erase(id);
    }

    void Context::destroy() {
        destroyRenderer();

        ALCdevice* device = alcGetContextsDevice(alcContext);
        alcMakeContextCurrent(NULL);
        alcDestroyContext(alcContext);
        alcCloseDevice(device);

        logger->log(NICKEL2_INFO, "alc context successfully destroyed.");

        if (windows.size() != 0) {
            logger->log(NICKEL2_WARNING, "there are still active windows.");
        }

        if (context == this)
            context = nullptr;

        logger->log(NICKEL2_INFO, "context successfully destroyed.");
        logger->destroy();
        delete logger;
        glfwTerminate();
    }
}