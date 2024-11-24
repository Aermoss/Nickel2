#include <Nickel2/nkpch.hpp>
#include <Nickel2/Audio/AudioSystem.hpp>

namespace Nickel2 {
    std::unique_ptr<AudioAPI> AudioSystem::api = AudioAPI::Create();
    std::vector<AudioListener*> AudioSystem::listeners;
    std::vector<AudioSource*> AudioSystem::sources;

    void AudioSystem::PushListener(AudioListener* listener) {
        NK_CORE_ASSERT(listeners.size() < 1, "Only one audio listener is allowed at a time.");
        listeners.push_back(listener);
    }

    void AudioSystem::PushSource(AudioSource* source) {
        sources.push_back(source);
    }

    void AudioSystem::PopListener(AudioListener* listener) {
        std::vector<AudioListener*>::iterator result = std::find(listeners.begin(), listeners.end(), listener);
        if (result != listeners.end()) listeners.erase(result);
    }

    void AudioSystem::PopSource(AudioSource* source) {
        std::vector<AudioSource*>::iterator result = std::find(sources.begin(), sources.end(), source);
        if (result != sources.end()) sources.erase(result);
    }

    void AudioSystem::Update(float deltaTime) {
        for (AudioListener* listener : listeners)
            listener->Update(deltaTime);

        for (AudioSource* source : sources)
            source->Update(deltaTime);
    }
}