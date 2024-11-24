#include <Nickel2/nkpch.hpp>
#include <Nickel2/Audio/OpenAL/OpenALListener.hpp>
#include <Nickel2/Physics/PhysicsBody.hpp>
#include <Nickel2/Scene/Entity.hpp>

namespace Nickel2 {
    namespace Utils {
        static uint32_t ToOpenALDistanceModel(DistanceModel distanceModel) {
            switch (distanceModel) {
                case DistanceModel::InverseDistance: return AL_INVERSE_DISTANCE;
                case DistanceModel::InverseDistanceClamped: return AL_INVERSE_DISTANCE_CLAMPED;
                case DistanceModel::LinearDistance: return AL_LINEAR_DISTANCE;
                case DistanceModel::LinearDistanceClamped: return AL_LINEAR_DISTANCE_CLAMPED;
                case DistanceModel::ExponentDistance: return AL_EXPONENT_DISTANCE;
                case DistanceModel::ExponentDistanceClamped: return AL_EXPONENT_DISTANCE_CLAMPED;
                default: return AL_NONE;
            }
        }

        static DistanceModel FromOpenALDistanceModel(int32_t distanceModel) {
            switch (distanceModel) {
                case AL_INVERSE_DISTANCE: return DistanceModel::InverseDistance;
                case AL_INVERSE_DISTANCE_CLAMPED: return DistanceModel::InverseDistanceClamped;
                case AL_LINEAR_DISTANCE: return DistanceModel::LinearDistance;
                case AL_LINEAR_DISTANCE_CLAMPED: return DistanceModel::LinearDistanceClamped;
                case AL_EXPONENT_DISTANCE: return DistanceModel::ExponentDistance;
                case AL_EXPONENT_DISTANCE_CLAMPED: return DistanceModel::ExponentDistanceClamped;
                default: return DistanceModel::None;
            }
        }
    }
    OpenALListener::OpenALListener(Entity* entity, DistanceModel distanceModel) : AudioListener(entity) {
        this->SetDistanceModel(distanceModel);
        AudioSystem::PushListener(this);
    }

    OpenALListener::~OpenALListener() {
        AudioSystem::PopListener(this);
    }

    void OpenALListener::SetDistanceModel(DistanceModel distanceModel) {
        alDistanceModel(Utils::ToOpenALDistanceModel(distanceModel));
    }

    DistanceModel OpenALListener::GetDistanceModel() {
        return Utils::FromOpenALDistanceModel(alGetInteger(AL_DISTANCE_MODEL));
    }

    void OpenALListener::Update(float deltaTime) {
        glm::vec3 translation = entity->GetComponent<TransformComponent>().GetTranslation();
        glm::mat4 view = entity->GetComponent<CameraComponent>()->GetView();
        glm::vec3 up = glm::vec3(view[1].x, view[1].y, -view[1].z);
        glm::vec3 front = glm::vec3(view[2].x, view[2].y, -view[2].z);
        glm::vec3 velocity = glm::vec3(0.0f);
        
        if (entity->HasComponent<RigidBodyComponent>())
            velocity = entity->GetComponent<RigidBodyComponent>()->GetLinearVelocity();

        float rotation[] = { front.x, front.y, front.z, up.x, up.y, up.z };
        alListener3f(AL_POSITION, translation.x, translation.y, translation.z);
        alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        alListenerfv(AL_ORIENTATION, rotation);
    }
}