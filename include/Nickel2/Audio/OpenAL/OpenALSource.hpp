#pragma once

#include <Nickel2/Audio/AudioSource.hpp>

#include <al/al.h>
#include <al/alc.h>
#include <al/alut.h>

namespace Nickel2 {
    class Entity;

    class OpenALSource : public AudioSource {
        private:
            uint32_t buffer, source;
            float timeScale;

            virtual void Update(float deltaTime) override;

        public:
            OpenALSource(Entity* entity, const char* filePath, bool looping, float gain, float pitch, float rollOffFactor, float refDistance, float maxDistance);
            ~OpenALSource();

            virtual void SetLooping(bool looping) override;
            virtual void SetPitch(float pitch) override;
            virtual void SetGain(float gain) override;
            virtual void SetRollOffFactor(float rollOffFactor) override;
            virtual void SetRefDistance(float refDistance) override;
            virtual void SetMaxDistance(float maxDistance) override;

            virtual bool GetLooping() override;
            virtual float GetPitch() override;
            virtual float GetGain() override;
            virtual float GetRollOffFactor() override;
            virtual float GetRefDistance() override;
            virtual float GetMaxDistance() override;

            virtual void Play() override;
            virtual void Pause() override;
            virtual void Stop() override;

            virtual bool IsPlaying() override;
            virtual bool IsPaused() override;
            virtual bool IsStopped() override;
    };
}