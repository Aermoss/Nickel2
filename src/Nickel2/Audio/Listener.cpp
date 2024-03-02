#include <Nickel2/nkpch.hpp>
#include <Nickel2/Audio/Listener.hpp>

namespace Nickel2 {
    uint32_t Listener::genUniqueSourceID() {
        uint32_t id = 0;
        while (sources.find(id) != sources.end()) id++;
        return id;
    }

    Listener::Listener(Camera* camera, Transform* transform)
        : camera(camera), transform(transform) {

    }

    Listener::~Listener() {

    }

    uint32_t Listener::addSource(Source* source) {
        uint32_t id = genUniqueSourceID();
        sources[id] = source;
        return id;
    }

    void Listener::removeSource(uint32_t id) {
        sources.erase(id);
    }

    void Listener::setCamera(Camera* camera) {
        this->camera = camera;
    }

    Camera* Listener::getCamera() {
        return camera;
    }

    void Listener::update() {
        for (const auto& [key, value] : sources)
            value->update();

        glm::vec3 front = camera != nullptr ? camera->front : glm::vec3(0.0f);
        glm::vec3 translation = transform->getTranslation();

        float rotation[] = { front.x, front.y, front.z, 0.0f, 1.0f, 0.0f };
        alListener3f(AL_POSITION, translation.x, translation.y, translation.z);
        alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
        alListenerfv(AL_ORIENTATION, rotation);
    }

    void Listener::destroy() {

    }
}