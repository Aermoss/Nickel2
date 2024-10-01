#include <Nickel2/nkpch.hpp>
#include <Nickel2/Scene/Component.hpp>
#include <Nickel2/Scene/Entity.hpp>

namespace Nickel2 {
    IDComponent::IDComponent(Entity* entity, uint64_t id) : id(id) {}
    IDComponent::~IDComponent() {}
    
    NameComponent::NameComponent(Entity* entity, const std::string& name) : name(name) {}
    NameComponent::~NameComponent() {}

    DirectionalLightComponent::DirectionalLightComponent(Entity* entity) {}
    DirectionalLightComponent::~DirectionalLightComponent() {}

    RelationshipComponent::RelationshipComponent(Entity* entity) : entity(entity) {}
    RelationshipComponent::~RelationshipComponent() {}

    void RelationshipComponent::SetParent(Entity* parent) {
        if (this->parent != nullptr)
            this->parent->GetComponent<RelationshipComponent>().RemoveChild(entity);

        this->parent = parent;

        if (this->parent != nullptr)
            this->parent->GetComponent<RelationshipComponent>().AddChild(entity);
    }

    Entity* RelationshipComponent::GetParent() const {
        return parent;
    }

    bool RelationshipComponent::HasParent() const {
        return parent != nullptr;
    }

    void RelationshipComponent::AddChild(Entity* child) {
        NK_CORE_ASSERT(std::find(children.begin(), children.end(), child) == children.end(), "Entity is already a child of this entity.");
        children.push_back(child);
        child->GetComponent<RelationshipComponent>().parent = entity;
    }

    void RelationshipComponent::RemoveChild(Entity* child) {
        auto it = std::find(children.begin(), children.end(), child);
        NK_CORE_ASSERT(it != children.end(), "Entity is not a child of this entity.");
        children.erase(it);
        child->GetComponent<RelationshipComponent>().parent = nullptr;
    }

    TransformComponent::TransformComponent(Entity* entity) : entity(entity) {}
    TransformComponent::~TransformComponent() {}

    void TransformComponent::Translate(const glm::vec3& translation) {
        this->translation += translation;
    }

    void TransformComponent::Rotate(const glm::vec3& rotation) {
        this->rotationEuler += rotation;
        this->rotation = glm::quat(this->rotationEuler);
    }

    void TransformComponent::Scale(const glm::vec3& scale) {
        this->_scale *= scale;
    }

    void TransformComponent::Reset() {
        this->overrideState = false;
        this->translation = { 0.0f, 0.0f, 0.0f };
        this->_scale = { 1.0f, 1.0f, 1.0f };
        this->rotationEuler = { 0.0f, 0.0f, 0.0f };
        this->rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
        this->matrix = glm::mat4(1.0f);
    }

    void TransformComponent::DecomposeTransform(const glm::mat4& transform) {
        Math::DecomposeTransform(transform, this->translation, this->rotation, this->_scale);
        this->rotationEuler = glm::eulerAngles(this->rotation);
    }

    void TransformComponent::OverrideTransform(const glm::mat4& transform) {
        this->matrix = transform;
        this->overrideState = true;
    }

    bool TransformComponent::IsOverridden() const {
        return this->overrideState;
    }

    glm::mat4 TransformComponent::GetWorldTransform() const {
        return !(entity->GetComponent<RelationshipComponent>().HasParent()) ? GetLocalTransform() \
             : (entity->GetComponent<RelationshipComponent>().GetParent()->GetComponent<TransformComponent>().GetWorldTransform() * GetLocalTransform());
    }

    glm::mat4 TransformComponent::GetLocalTransform() const {
        return IsOverridden() ? this->matrix : (glm::translate(glm::mat4(1.0f), this->translation)
             * glm::toMat4(this->rotation) * glm::scale(glm::mat4(1.0f), this->_scale));
    }
    
    glm::vec3 TransformComponent::GetTranslation() const {
        return this->translation;
    }

    void TransformComponent::SetTranslation(const glm::vec3& translation) {
        this->translation = translation;
    }

    glm::vec3 TransformComponent::GetScale() const {
        return this->_scale;
    }

    void TransformComponent::SetScale(const glm::vec3& scale) {
        this->_scale = scale;
    }

    glm::vec3 TransformComponent::GetRotationEuler() const {
        return this->rotationEuler;
    }

    void TransformComponent::SetRotationEuler(const glm::vec3& rotation) {
        this->rotationEuler = rotation;
        this->rotation = glm::quat(this->rotationEuler);
    }

    glm::quat TransformComponent::GetRotation() const {
        return this->rotation;
    }

    void TransformComponent::SetRotation(const glm::quat& rotation) {
        glm::vec3 originalEuler = rotationEuler;
        this->rotation = rotation;
        this->rotationEuler = glm::eulerAngles(this->rotation);

        if ((fabs(rotationEuler.x - originalEuler.x) == glm::pi<float>())
         && (fabs(rotationEuler.z - originalEuler.z) == glm::pi<float>())) {
            rotationEuler.x = originalEuler.x;
            rotationEuler.y = glm::pi<float>() - rotationEuler.y;
            rotationEuler.z = originalEuler.z;
        }
    }

    SubmeshComponent::SubmeshComponent(Entity* entity, std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material& material) {
        submesh = new Submesh(entity, vertices, indices, material);
    }

    SubmeshComponent::~SubmeshComponent() {}

    void SubmeshComponent::Destroy() {
        submesh->Destroy();
        delete submesh;
    }

    MeshComponent::MeshComponent(Entity* entity, const std::string& path) {
        mesh = new Mesh(entity, path);
    }

    MeshComponent::~MeshComponent() {}

    void MeshComponent::Destroy() {
        mesh->Destroy();
        delete mesh;
    }

    CameraComponent::CameraComponent(Entity* entity, Window* window, float fov, float nearPlane, float farPlane, uint32_t flags) {
        camera = new Camera(entity, window, fov, nearPlane, farPlane, flags);
    }

    CameraComponent::~CameraComponent() {}

    void CameraComponent::Destroy() {
        camera->Destroy();
        delete camera;
    }

    ListenerComponent::ListenerComponent(Entity* entity, DistanceModel distanceModel) {
        listener = new Listener(entity, distanceModel);
    }

    ListenerComponent::~ListenerComponent() {}

    void ListenerComponent::Destroy() {
        listener->Destroy();
        delete listener;
    }

    SourceComponent::SourceComponent(Entity* entity, Listener* listener, const char* filePath, bool looping, float gain, float pitch, float rollOffFactor, float refDistance, float maxDistance) {
        source = new Source(entity, listener, filePath, looping, gain, pitch, rollOffFactor, refDistance, maxDistance);
    }

    SourceComponent::SourceComponent(Entity* entity, ListenerComponent& listener, const char* filePath, bool looping, float gain, float pitch, float rollOffFactor, float refDistance, float maxDistance) {
        source = new Source(entity, listener.listener, filePath, looping, gain, pitch, rollOffFactor, refDistance, maxDistance);
    }

    SourceComponent::~SourceComponent() {}

    void SourceComponent::Destroy() {
        source->Destroy();
        delete source;
    }
}