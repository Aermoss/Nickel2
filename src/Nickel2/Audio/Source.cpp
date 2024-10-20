#include <Nickel2/nkpch.hpp>
#include <Nickel2/Audio/Source.hpp>
#include <Nickel2/Scene/Entity.hpp>
#include <Nickel2/Core/Application.hpp>

namespace Nickel2 {
    Source::Source(Entity* entity, Listener* listener, const char* filePath, bool looping, float pitch, float gain, float rollOffFactor, float refDistance, float maxDistance)
        : entity(entity), listener(listener), lastTranslation(0.0f), lastTimeScale(Application::Get().GetTimeScale()), \
          pitch(pitch * lastTimeScale), gain(gain), looping(looping) {

        void* data;
        int32_t format, size, freq;
        alutLoadWAVFile((ALbyte*) filePath, &format, &data, &size, &freq, nullptr);

        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, data, size, freq);

        int32_t error = alGetError();
        
        if (error != 0)
            Nickel2::Logger::Log(Nickel2::Logger::Level::Error, "Source", (std::string(filePath) + " (" + std::to_string(error) + ")").c_str());

        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, buffer);
        alSourcei(source, AL_LOOPING, looping);
        alSourcef(source, AL_ROLLOFF_FACTOR, rollOffFactor);
        alSourcef(source, AL_REFERENCE_DISTANCE, refDistance);
        alSourcef(source, AL_MAX_DISTANCE, maxDistance);
        alSourcef(source, AL_PITCH, pitch);
        alSourcef(source, AL_GAIN, gain);

        id = listener->AddSource(this);
    }

    Source::~Source() {

    }

    void Source::SetLooping(bool looping) {
        alSourcei(source, AL_LOOPING, looping);
        this->looping = looping;
    }

    void Source::SetPitch(float pitch) {
        lastTimeScale = Application::Get().GetTimeScale();
        alSourcef(source, AL_PITCH, pitch * lastTimeScale);
        this->pitch = pitch * lastTimeScale;
    }

    void Source::SetGain(float gain) {
        alSourcef(source, AL_GAIN, gain);
        this->gain = gain;
    }

    void Source::SetRollOffFactor(float rollOffFactor) {
        alSourcef(source, AL_ROLLOFF_FACTOR, rollOffFactor);
        this->rollOffFactor = rollOffFactor;
    }

    void Source::SetRefDistance(float refDistance) {
        alSourcef(source, AL_REFERENCE_DISTANCE, refDistance);
        this->refDistance = refDistance;
    }

    void Source::SetMaxDistance(float maxDistance) {
        alSourcef(source, AL_MAX_DISTANCE, maxDistance);
        this->maxDistance = maxDistance;
    }

    bool Source::GetLooping() {
        return looping;
    }

    float Source::GetPitch() {
        return pitch / lastTimeScale;
    }

    float Source::GetGain() {
        return gain;
    }

    float Source::GetRollOffFactor() {
        return rollOffFactor;
    }

    float Source::GetRefDistance() {
        return refDistance;
    }

    float Source::GetMaxDistance() {
        return maxDistance;
    }

    void Source::Play() {
        alSourcePlay(source);
    }

    void Source::Pause() {
        alSourcePause(source);
    }

    void Source::Stop() {
        alSourceStop(source);
    }

    bool Source::Playing() {
        int32_t state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return (state == AL_PLAYING);
    }

    bool Source::Paused() {
        int32_t state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return (state == AL_PAUSED);
    }

    bool Source::Stopped() {
        int32_t state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return (state == AL_STOPPED);
    }

    void Source::SetVelocity(const glm::vec3& velocity) {
        this->velocity = velocity;
        dirtyVelocity = true;
    }

    glm::vec3 Source::GetVelocity() {
        return velocity;
    }

    void Source::ResetVelocity() {
        dirtyVelocity = false;
    }

    void Source::Update(float deltaTime) {
        glm::vec3 translation = entity->GetComponent<TransformComponent>().GetTranslation();

        if (!dirtyVelocity)
            velocity = glm::round(1 / deltaTime * (translation - lastTranslation));

        float timeScale = Application::Get().GetTimeScale();

        if (timeScale != lastTimeScale) {
            pitch = pitch / lastTimeScale * timeScale;
            alSourcef(source, AL_PITCH, pitch);
            lastTimeScale = timeScale;
        }

        alSource3f(source, AL_POSITION, translation.x, translation.y, translation.z);
        alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        lastTranslation = translation;
    }

    void Source::Destroy() {
        listener->RemoveSource(id);
        alDeleteBuffers(1, &buffer);
        alDeleteSources(1, &source);
    }
}