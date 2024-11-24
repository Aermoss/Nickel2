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
#include "../Math/Math.hpp"

#include <cstdint>
#include <string>

namespace Nickel2 {
    class Entity;
    class Camera;
    class Window;
    class Submesh;
    class Mesh;

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
            glm::vec3 _scale = { 1.0f, 1.0f, 1.0f };
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

        void Destroy();

        Submesh* operator->() {
            return submesh;
        }
    };

    struct MeshComponent {
        Mesh* mesh;

        MeshComponent(Entity* entity, const std::string& path);
        ~MeshComponent();

        void Destroy();

        Mesh* operator->() {
            return mesh;
        }
    };

    struct CameraComponent {
        Camera* camera;

        CameraComponent(Entity* entity, Window* window, float fov = 90.0f, float nearPlane = 0.01f, float farPlane = 1000.0f, uint32_t flags = NK_EULER_OVERRIDE);
        ~CameraComponent();

        void Destroy();

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

        }
    };
}