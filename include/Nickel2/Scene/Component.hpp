#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/io.hpp>

#include "../Core/Logger.hpp"
#include "../Audio/AudioSystem.hpp"
#include "../Renderer/Material.hpp"
#include "../Renderer/Mesh.hpp"
#include "../Renderer/Submesh.hpp"
#include "../Renderer/Camera.hpp"
#include "../Physics/PhysicsTypes.hpp"
#include "../Physics/MeshCookingFactory.hpp"
#include "../Physics/ColliderMaterial.hpp"
#include "../Math/Math.hpp"

#include <cstdint>
#include <string>

namespace Nickel2 {
    class Entity;
    class Camera;
    class Window;
    class Submesh;
    class Mesh;
    class PhysicsScene;
    class PhysicsBody;

    struct Material;
    
    struct IDComponent {
        uint64_t id = 0;

        IDComponent(Entity* entity, uint64_t id);
        ~IDComponent();
    };

    struct NameComponent {
        std::string name;

        NameComponent(Entity* entity, const std::string& name);
        ~NameComponent();
    };

    struct PointLightComponent {
        glm::vec3 position = { 0.0f, 0.0f, 0.0f }, color = { 1.0f, 1.0f, 1.0f };
        float brightness = 1.0f;
        
        PointLightComponent(Entity* entity);
        ~PointLightComponent();
    };

    struct DirectionalLightComponent {
        glm::vec3 direction = { 0.0f, 0.0f, 0.0f };
        bool castShadows = true, followCamera = true;
        
        float distance = 20.0f, size = 35.0f, \
            nearPlane = 0.1f, farPlane = 100.0f;
        
        DirectionalLightComponent(Entity* entity);
        ~DirectionalLightComponent();
    };

    struct RelationshipComponent {
        private:
            Entity *entity = nullptr, *parent = nullptr;
            std::vector<Entity*> children;

        public:
            RelationshipComponent(Entity* entity);
            ~RelationshipComponent();

            void SetParent(Entity* parent);
            Entity* GetParent() const;
            bool HasParent() const;

            void AddChild(Entity* child);
            void RemoveChild(Entity* child);
    };

    struct TransformComponent {
        private:
            Entity* entity;
            bool overrideState = false;

            glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
            glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
            glm::vec3 rotationEuler = { 0.0f, 0.0f, 0.0f };
            glm::quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
            glm::mat4 matrix = glm::mat4(1.0f);

        public:
            TransformComponent(Entity* entity);
            ~TransformComponent();

            void Translate(const glm::vec3& translation);
            void Rotate(const glm::vec3& rotation);
            void Scale(const glm::vec3& scale);

            void Reset();

            void DecomposeTransform(const glm::mat4& transform);
            void OverrideTransform(const glm::mat4& transform);
            bool IsOverridden() const;

            glm::mat4 GetWorldTransform() const;
            glm::mat4 GetLocalTransform() const;

            glm::vec3 GetTranslation() const;
            void SetTranslation(const glm::vec3& translation);

            glm::vec3 GetScale() const;
            void SetScale(const glm::vec3& scale);

            glm::vec3 GetRotationEuler() const;
            void SetRotationEuler(const glm::vec3& rotation);

            glm::quat GetRotation() const;
            void SetRotation(const glm::quat& rotation);
    };

    struct SubmeshComponent {
        Submesh* submesh;

        SubmeshComponent(Entity* entity, std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material& material);
        ~SubmeshComponent();

        Submesh* operator->() {
            return submesh;
        }
    };

    struct MeshComponent {
        Mesh* mesh;

        MeshComponent(Entity* entity, const std::string& path);
        ~MeshComponent();

        Mesh* operator->() {
            return mesh;
        }
    };

    struct CameraComponent {
        Camera* camera;

        CameraComponent(Entity* entity, float fov = 90.0f, float nearPlane = 0.01f, float farPlane = 1000.0f, uint32_t flags = NK_EULER_OVERRIDE);
        ~CameraComponent();

        Camera* operator->() {
            return camera;
        }
    };

    struct ListenerComponent {
        std::shared_ptr<AudioListener> listener;

        ListenerComponent(Entity* entity, DistanceModel distanceModel = DistanceModel::InverseDistance);
        ~ListenerComponent();

        AudioListener* operator->() {
            return listener.get();
        }
    };

    struct SourceComponent {
        std::shared_ptr<AudioSource> source;

        SourceComponent(Entity* entity, const char* filePath, bool looping = false, float pitch = 1.0f, float gain = 1.0f, float rollOffFactor = 1.0f, float refDistance = 1.0f, float maxDistance = FLT_MAX);
        ~SourceComponent();

        AudioSource* operator->() {
            return source.get();
        }
    };

    struct RigidBodyComponent {
        uint32_t layerId = 0;
        bool enableDynamicTypeChange = false, disableGravity = false, isTrigger = false;
        float maxLinearVelocity = 500.0f, maxAngularVelocity = 50.0f;
        float mass = 1.0f, linearDrag = 0.01f, angularDrag = 0.05f;

        ActorAxis lockedAxes = ActorAxis::None;
        CollisionDetection collisionDetection = CollisionDetection::Discrete;
        BodyType bodyType = BodyType::Static;

        glm::vec3 initialLinearVelocity = glm::vec3(0.0f);
        glm::vec3 initialAngularVelocity = glm::vec3(0.0f);

        std::shared_ptr<PhysicsBody> body;
        std::shared_ptr<PhysicsScene> scene;

        RigidBodyComponent(Entity* entity, float mass = 1.0f, BodyType bodyType = BodyType::Static, bool gravity = true);
        ~RigidBodyComponent();

        PhysicsBody* operator->() {
            return body.get();
        }
    };

    struct CompoundColliderComponent {
        std::vector<uint64_t> compoundedColliderEntities;
        bool includeStaticChildColliders = true, isImmutable = true;

        CompoundColliderComponent(Entity* entity) {}
        ~CompoundColliderComponent() {}

        void PushChildCollider(Entity* childCollider);
        void PopChildCollider();
    };

    struct BoxColliderComponent {
        glm::vec3 halfSize, offset;
        ColliderMaterial material;

        BoxColliderComponent(Entity* entity, const glm::vec3& halfSize = glm::vec3(0.5f), const glm::vec3& offset = glm::vec3(0.0f), ColliderMaterial material = {}) : halfSize(halfSize), offset(offset), material(material) {}
        ~BoxColliderComponent() {}
    };

    struct SphereColliderComponent {
        float radius;
        ColliderMaterial material;
        glm::vec3 offset;

        SphereColliderComponent(Entity* entity, float radius = 0.5f, const glm::vec3& offset = glm::vec3(0.0f), ColliderMaterial material = {}) : radius(radius), material(material), offset(offset) {}
        ~SphereColliderComponent() {}
    };

    struct CapsuleColliderComponent {
        float radius, halfHeight;
        ColliderMaterial material;
        glm::vec3 offset;

        CapsuleColliderComponent(Entity* entity, float radius = 0.5f, float halfHeight = 0.5f, const glm::vec3& offset = glm::vec3(0.0f), ColliderMaterial material = {}) : radius(radius), halfHeight(halfHeight), material(material), offset(offset) {}
        ~CapsuleColliderComponent() {}
    };

    struct MeshColliderComponent {
        CollisionComplexity complexity;
        ColliderMaterial material;

        MeshColliderComponent(Entity* entity, CollisionComplexity complexity = CollisionComplexity::Simple, ColliderMaterial material = {}) : complexity(complexity), material(material) {};
        ~MeshColliderComponent() {};
    };
}