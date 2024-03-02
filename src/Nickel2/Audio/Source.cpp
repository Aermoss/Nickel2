#include <Nickel2/nkpch.hpp>
#include <Nickel2/Audio/Source.hpp>

namespace Nickel2 {
    Source::Source(Listener* listener, Transform* transform, const char* filePath, bool looping, float pitch, float gain)
        : listener(listener), pitch(pitch), gain(gain), looping(looping), transform(transform) {
        void* data;
        int32_t format, size, freq;
        alutLoadWAVFile((ALbyte*) filePath, &format, &data, &size, &freq, nullptr);

        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, data, size, freq);

        int32_t error = alGetError();
        if (error != 0) getContext()->logger->log(NICKEL2_ERROR, ("openal: " + std::string(filePath) + " (" + std::to_string(error) + ")").c_str());

        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, buffer);
        alSourcei(source, AL_LOOPING, looping);
        alSourcef(source, AL_PITCH, pitch);
        alSourcef(source, AL_GAIN, gain);

        id = listener->addSource(this);
    }

    Source::~Source() {

    }

    void Source::setLooping(bool looping) {
        alSourcei(source, AL_LOOPING, looping);
        this->looping = looping;
    }

    void Source::setPitch(float pitch) {
        alSourcef(source, AL_PITCH, pitch);
        this->pitch = pitch;
    }

    void Source::setGain(float gain) {
        alSourcef(source, AL_GAIN, gain);
        this->gain = gain;
    }

    bool Source::getLooping() {
        return looping;
    }

    float Source::getPitch() {
        return pitch;
    }

    float Source::getGain() {
        return gain;
    }

    void Source::play() {
        alSourcePlay(source);
    }

    void Source::pause() {
        alSourcePause(source);
    }

    void Source::stop() {
        alSourceStop(source);
    }

    bool Source::playing() {
        int32_t state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return (state == AL_PLAYING);
    }

    bool Source::paused() {
        int32_t state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return (state == AL_PAUSED);
    }

    bool Source::stopped() {
        int32_t state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return (state == AL_STOPPED);
    }

    void Source::update() {
        glm::vec3 translation = transform->getTranslation();
        alSource3f(source, AL_POSITION, translation.x, translation.y, translation.z);
        alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    }

    void Source::destroy() {
        listener->removeSource(id);
        alDeleteBuffers(1, &buffer);
        alDeleteSources(1, &source);
    }
}