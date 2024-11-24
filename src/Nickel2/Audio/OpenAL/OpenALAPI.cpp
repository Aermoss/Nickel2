#include <Nickel2/nkpch.hpp>
#include <Nickel2/Audio/OpenAL/OpenALAPI.hpp>

namespace Nickel2 {
    OpenALAPI::OpenALAPI() {}
    OpenALAPI::~OpenALAPI() {}

    void OpenALAPI::Initialize() {
        if (!alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"))
            Nickel2::Logger::Log(Nickel2::Logger::Level::Error, "OpenAL", "ALC enumeration extension not available.");

        const char* defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
        ALCdevice* device = alcOpenDevice(defaultDeviceName);

        if (!device) {
            Nickel2::Logger::Log(Nickel2::Logger::Level::Error, "OpenAL", ("Unable to open default audio device: \"" + std::string(defaultDeviceName) + "\".").c_str());
        } else {
            Nickel2::Logger::Log(Nickel2::Logger::Level::Info, "OpenAL", ("Found audio device: \"" + std::string(defaultDeviceName) + "\".").c_str());
            context = alcCreateContext(device, NULL);

            if (!alcMakeContextCurrent(context))
                Nickel2::Logger::Log(Nickel2::Logger::Level::Error, "OpenAL", "Failed to make ALC context current.");
        }
    }

    void OpenALAPI::Terminate() {
        alcCloseDevice(alcGetContextsDevice(context));
        alcDestroyContext(context);
        alcMakeContextCurrent(nullptr);
    }
}