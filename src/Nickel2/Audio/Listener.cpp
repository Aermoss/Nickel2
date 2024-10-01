#include <Nickel2/nkpch.hpp>
#include <Nickel2/Audio/Listener.hpp>
#include <Nickel2/Scene/Entity.hpp>

namespace Nickel2 {
    uint32_t Listener::GenUniqueSourceID() {
        uint32_t id = 0;
        while (sources.find(id) != sources.end()) id++;
        return id;
    }

    Listener::Listener(Entity* entity, DistanceModel distanceModel) : entity(entity), lastTranslation(0.0f) {
        alDistanceModel((int32_t) distanceModel);
    }

    Listener::~Listener() {

    }

    uint32_t Listener::AddSource(Source* source) {
        uint32_t id = GenUniqueSourceID();
        sources[id] = source;
        return id;
    }

    void Listener::RemoveSource(uint32_t id) {
        sources.erase(id);
    }

    void Listener::SetDistanceModel(int32_t distanceModel) {
        alDistanceModel(distanceModel);
    }

    void Listener::SetVelocity(const glm::vec3& velocity) {
        this->velocity = velocity;
        dirtyVelocity = true;
    }

    glm::vec3 Listener::GetVelocity() {
        return velocity;
    }

    void Listener::ResetVelocity() {
        dirtyVelocity = false;
    }

    void Listener::Update(float deltaTime) {
        for (const auto& [key, value] : sources)
            value->Update(deltaTime);

        glm::mat4 view = entity->GetComponent<CameraComponent>()->GetView();
        glm::vec3 up = glm::vec3(view[1].x, view[1].y, -view[1].z);
        glm::vec3 front = glm::vec3(view[2].x, view[2].y, -view[2].z);
        glm::vec3 translation = entity->GetComponent<TransformComponent>().GetTranslation();

        if (!dirtyVelocity)
            velocity = glm::round(1 / deltaTime * (translation - lastTranslation));

        float rotation[] = { front.x, front.y, front.z, up.x, up.y, up.z };
        alListener3f(AL_POSITION, translation.x, translation.y, translation.z);
        alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        alListenerfv(AL_ORIENTATION, rotation);
        lastTranslation = translation;
    }

    void Listener::Destroy() {

    }
}