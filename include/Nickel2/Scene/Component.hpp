#pragma once

#include <cstdint>
#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/io.hpp>

#include "../Core/Context.hpp"
#include "../Core/Logger.hpp"
#include "../Audio/Listener.hpp"
#include "../Audio/Source.hpp"

namespace Nickel2 {
    class Entity;
    class Camera;
    class Window;
    class Mesh;
    
    struct IDComponent {
        uint64_t id;

        IDComponent(Entity* entity, uint64_t id);
        ~IDComponent();
    };

    struct NameComponent {
        std::string name;

        NameComponent(Entity* entity, const std::string& name);
        ~NameComponent();
    };

    struct TransformComponent {
        Transform* transform;

        TransformComponent(Entity* entity);
        ~TransformComponent();

        void destroy();

        Transform* operator->() {
            return transform;
        }
    };

    struct MeshComponent {
        Mesh* mesh;

        MeshComponent(Entity* entity, const std::string& path);
        ~MeshComponent();

        void destroy();

        Mesh* operator->() {
            return mesh;
        }
    };

    struct CameraComponent {
        Camera* camera;

        CameraComponent(Entity* entity, Window* window, float fov = 90.0f, float near = 0.01f, float far = 1000.0f, uint32_t flags = NICKEL2_EULER_OVERRIDE);
        ~CameraComponent();

        void destroy();

        Camera* operator->() {
            return camera;
        }
    };

    struct ListenerComponent {
        Listener* listener;

        ListenerComponent(Entity* entity, Camera* camera);
        ListenerComponent(Entity* entity, CameraComponent& camera);
        ~ListenerComponent();

        void destroy();

        Listener* operator->() {
            return listener;
        }
    };

    struct SourceComponent {
        Source* source;

        SourceComponent(Entity* entity, Listener* listener, const char* filePath, bool looping = false, float pitch = 1.0f, float gain = 1.0f);
        SourceComponent(Entity* entity, ListenerComponent& listener, const char* filePath, bool looping = false, float pitch = 1.0f, float gain = 1.0f);
        ~SourceComponent();

        void destroy();

        Source* operator->() {
            return source;
        }
    };
}