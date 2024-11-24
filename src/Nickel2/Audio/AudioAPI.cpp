#include <Nickel2/nkpch.hpp>
#include <Nickel2/Audio/AudioAPI.hpp>

#include <Nickel2/Audio/OpenAL/OpenALAPI.hpp>

namespace Nickel2 {
    std::unique_ptr<AudioAPI> AudioAPI::Create() {
        switch (AudioAPI::GetAPI()) {
            case AudioAPIType::OpenAL: return std::make_unique<OpenALAPI>();
            default: return nullptr;
        }
    }
}