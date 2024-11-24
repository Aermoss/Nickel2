#pragma once

#include "AudioAPI.hpp"
#include "AudioListener.hpp"
#include "AudioSource.hpp"

namespace Nickel2 {
    class AudioSystem {
        private:
            static std::unique_ptr<AudioAPI> api;
            static std::vector<AudioListener*> listeners;
            static std::vector<AudioSource*> sources;

            static void Update(float deltaTime);

        public:
            static void Initialize() { api->Initialize(); }
            static void Terminate() { api->Terminate(); }

            static void PushListener(AudioListener* listener);
            static void PushSource(AudioSource* source);

            static void PopListener(AudioListener* listener);
            static void PopSource(AudioSource* source);

            static AudioAPI* GetAPI() { return api.get(); }

            friend class Application;
    };
}