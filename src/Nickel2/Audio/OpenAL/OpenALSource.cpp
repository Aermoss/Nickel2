#include <Nickel2/nkpch.hpp>
#include <Nickel2/Audio/OpenAL/OpenALSource.hpp>
#include <Nickel2/Physics/PhysicsBody.hpp>
#include <Nickel2/Core/Application.hpp>
#include <Nickel2/Scene/Entity.hpp>

namespace Nickel2 {
    OpenALSource::OpenALSource(Entity* entity, const char* filePath, bool looping, float pitch, float gain, float rollOffFactor, float refDistance, float maxDistance) : AudioSource(entity), timeScale(Application::Get().GetTimeScale()) {
        void* data;
        int32_t format, size, freq;
        alutLoadWAVFile((ALbyte*) filePath, &format, &data, &size, &freq, nullptr);

        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, data, size, freq);

        int32_t error = alGetError();
        
        if (error != 0)
            Nickel2::Logger::Log(Nickel2::Logger::Level::Error, "OpenALSource", (std::string(filePath) + " (" + std::to_string(error) + ")").c_str());

        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, buffer);
        alSourcei(source, AL_LOOPING, looping);
        alSourcef(source, AL_ROLLOFF_FACTOR, rollOffFactor);
        alSourcef(source, AL_REFERENCE_DISTANCE, refDistance);
        alSourcef(source, AL_MAX_DISTANCE, maxDistance);
        alSourcef(source, AL_PITCH, pitch);
        alSourcef(source, AL_GAIN, gain);
        AudioSystem::PushSource(this);
    }

    OpenALSource::~OpenALSource() {
        AudioSystem::PopSource(this);
        alDeleteBuffers(1, &buffer);
        alDeleteSources(1, &source);
    }

    void OpenALSource::SetLooping(bool looping) {
        alSourcei(source, AL_LOOPING, looping);
    }

    void OpenALSource::SetPitch(float pitch) {
        timeScale = Application::Get().GetTimeScale();
        alSourcef(source, AL_PITCH, pitch * timeScale);
    }

    void OpenALSource::SetGain(float gain) {
        alSourcef(source, AL_GAIN, gain);
    }

    void OpenALSource::SetRollOffFactor(float rollOffFactor) {
        alSourcef(source, AL_ROLLOFF_FACTOR, rollOffFactor);
    }

    void OpenALSource::SetRefDistance(float refDistance) {
        alSourcef(source, AL_REFERENCE_DISTANCE, refDistance);
    }

    void OpenALSource::SetMaxDistance(float maxDistance) {
        alSourcef(source, AL_MAX_DISTANCE, maxDistance);
    }

    bool OpenALSource::GetLooping() {
        int32_t looping;
        alGetSourcei(source, AL_LOOPING, &looping);
        return static_cast<bool>(looping);
    }

    float OpenALSource::GetPitch() {
        float pitch;
        alGetSourcef(source, AL_PITCH, &pitch);
        return pitch / timeScale;
    }

    float OpenALSource::GetGain() {
        float gain;
        alGetSourcef(source, AL_GAIN, &gain);
        return gain;
    }

    float OpenALSource::GetRollOffFactor() {
        float rollOffFactor;
        alGetSourcef(source, AL_ROLLOFF_FACTOR, &rollOffFactor);
        return rollOffFactor;
    }

    float OpenALSource::GetRefDistance() {
        float refDistance;
        alGetSourcef(source, AL_REFERENCE_DISTANCE, &refDistance);
        return refDistance;
    }

    float OpenALSource::GetMaxDistance() {
        float maxDistance;
        alGetSourcef(source, AL_MAX_DISTANCE, &maxDistance);
        return maxDistance;
    }

    void OpenALSource::Play() {
        alSourcePlay(source);
    }

    void OpenALSource::Pause() {
        alSourcePause(source);
    }

    void OpenALSource::Stop() {
        alSourceStop(source);
    }

    bool OpenALSource::IsPlaying() {
        int32_t state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return (state == AL_PLAYING);
    }

    bool OpenALSource::IsPaused() {
        int32_t state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return (state == AL_PAUSED);
    }

    bool OpenALSource::IsStopped() {
        int32_t state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return (state == AL_STOPPED);
    }

    void OpenALSource::Update(float deltaTime) {
        if (timeScale != Application::Get().GetTimeScale())
            this->SetPitch(this->GetPitch());

        glm::vec3 translation = entity->GetComponent<TransformComponent>().GetTranslation();
        glm::vec3 velocity = glm::vec3(0.0f);

        if (entity->HasComponent<RigidBodyComponent>())
            velocity = entity->GetComponent<RigidBodyComponent>()->GetLinearVelocity();

        alSource3f(source, AL_POSITION, translation.x, translation.y, translation.z);
        alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    }
}