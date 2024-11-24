#include <Nickel2/nkpch.hpp>
#include <Nickel2/Audio/AudioSystem.hpp>
#include <Nickel2/Audio/AudioListener.hpp>

#include <Nickel2/Audio/OpenAL/OpenALListener.hpp>

namespace Nickel2 {
    std::shared_ptr<AudioListener> AudioListener::Create(Entity* entity, DistanceModel distanceModel) {
        switch (AudioAPI::GetAPI()) {
            case AudioAPIType::OpenAL: return std::make_shared<OpenALListener>(entity, distanceModel);
            default: return nullptr;
        }
    }
}