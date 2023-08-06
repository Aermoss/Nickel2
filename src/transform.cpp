#include <nickel2/transform.hpp>

namespace nickel2 {
    Transform::Transform() {
        matrix = glm::mat4(1.0f);
        position = glm::vec3(0.0f);
        scale = glm::vec3(1.0f);
        rotation = glm::quat(glm::vec3(0.0f));
        dirtyDOF = dirtyMatrix = false;
        parent = nullptr;
    }

    Transform::~Transform() {

    }

    void Transform::setPosition(const glm::vec3& position) {
        this->position = position;
        dirtyDOF = true;
    }

    void Transform::setScale(const glm::vec3& scale) {
        this->scale = scale;
        dirtyDOF = true;
    }

    void Transform::setRotation(const glm::vec3& rotation) {
        this->rotation = glm::quat(rotation);
        dirtyDOF = true;
    }

    void Transform::setRotationQuat(const glm::quat& rotation) {
        this->rotation = rotation;
        dirtyDOF = true;
    }

    void Transform::rotateBy(const glm::vec3& rotation) {
        setRotation(getRotation() + rotation);
    }

    glm::vec3 Transform::getPosition() {
        return position;
    }

    glm::vec3 Transform::getWorldPosition() {
        return glm::vec3(matrix * glm::vec4(glm::vec3(0.0f), 1.0f));
    }

    glm::vec3 Transform::getScale() {
        return scale;
    }

    glm::vec3 Transform::getRotation() {
        return glm::eulerAngles(rotation);
    }

    glm::quat Transform::getRotationQuat() {
        return rotation;
    }

    glm::quat Transform::getWorldRotationQuat() {
        if (parent == nullptr) return getRotationQuat();
        else return parent->getWorldRotationQuat() * rotation;
    }

    void Transform::overrideMatrix(glm::mat4 matrix) {
        dirtyDOF = dirtyMatrix = true;
        matrix = matrix;
    }

    void Transform::updateWorldMatrix(bool dirtyParent) {
        bool dirty = dirtyDOF || dirtyParent;

        if (dirty && !dirtyMatrix) {
            if (parent) matrix = getParentMatrix() * getLocalMatrix();
            else matrix = getLocalMatrix();
        }

        for (Transform* child : children)
            child->updateWorldMatrix(dirty || dirtyMatrix);

        dirtyDOF = dirtyMatrix = false;
    }

    glm::mat4 Transform::getLocalMatrix() const {
        glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 R = glm::toMat4(rotation);
        glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
        return T * R * S;
    }

    glm::mat4 Transform::getParentMatrix() const {
        return parent->getWorldMatrix();
    }

    glm::mat4 Transform::getWorldMatrix() const {
        return matrix;
    }

    void Transform::reset() {
        matrix = glm::mat4(1.0f);
        position = glm::vec3(0.0f), scale = glm::vec3(0.0f);
        rotation = glm::quat(glm::vec3(0.0f));
        dirtyDOF = dirtyMatrix = false;
    }

    void Transform::destroy() {
        
    }
}