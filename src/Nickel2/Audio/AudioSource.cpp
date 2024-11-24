#include <Nickel2/nkpch.hpp>
#include <Nickel2/Audio/AudioSystem.hpp>
#include <Nickel2/Audio/AudioSource.hpp>

#include <Nickel2/Audio/OpenAL/OpenALSource.hpp>

namespace Nickel2 {
    std::shared_ptr<AudioSource> AudioSource::Create(Entity* entity, const char* filePath, bool looping, float gain, float pitch, float rollOffFactor, float refDistance, float maxDistance) {
        switch (AudioAPI::GetAPI()) {
            case AudioAPIType::OpenAL: return std::make_shared<OpenALSource>(entity, filePath, looping, gain, pitch, rollOffFactor, refDistance, maxDistance);
            default: return nullptr;
        }
    }
}