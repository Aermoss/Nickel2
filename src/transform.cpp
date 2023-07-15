#include "transform.hpp"

namespace nickel2 {
    Transform::Transform() {
        this->matrix = glm::mat4(1.0f);
        pivotX = glm::vec3(0.0f);
        pivotY = glm::vec3(0.0f);
        pivotZ = glm::vec3(0.0f);
        rotationVec = glm::vec3(0.0f);
        positionVec = glm::vec3(0.0f);
        scaleVec = glm::vec3(1.0f);
    }

    Transform::~Transform() {

    }

    void Transform::setMatrix(glm::mat4 matrix) {
        this->matrix = matrix;
    }

    void Transform::setRotation(glm::vec3 rotation) {
        rotationVec = rotation;
    }

    void Transform::setPosition(glm::vec3 position) {
        positionVec = position;
    }

    void Transform::setScale(glm::vec3 scale) {
        scaleVec = scale;
    }

    glm::vec3 Transform::getRotation() {
        return rotationVec;
    }

    glm::vec3 Transform::getPosition() {
        return positionVec;
    }

    glm::vec3 Transform::getScale() {
        return scaleVec;
    }

    void Transform::rotate(glm::vec3 value) {
        rotationVec += value;
    }

    void Transform::translate(glm::vec3 value) {
        bool updatePivot = (pivotX == positionVec && pivotY == positionVec && pivotZ == positionVec);
        positionVec += value;

        if (updatePivot) {
            pivotX = positionVec, pivotY = positionVec, pivotZ = positionVec;
        }
    }

    void Transform::scale(glm::vec3 value) {
        scaleVec *= value;
    }

    glm::mat4 Transform::get() {
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, pivotX);
        matrix = glm::rotate(matrix, glm::radians(rotationVec.x), glm::vec3(1.0f, 0.0f, 0.0f));        
        matrix = glm::translate(matrix, -pivotX);
        matrix = glm::translate(matrix, pivotY);
        matrix = glm::rotate(matrix, glm::radians(rotationVec.y), glm::vec3(0.0f, 1.0f, 0.0f));        
        matrix = glm::translate(matrix, -pivotY);
        matrix = glm::translate(matrix, pivotZ);
        matrix = glm::rotate(matrix, glm::radians(rotationVec.z), glm::vec3(0.0f, 0.0f, 1.0f));        
        matrix = glm::translate(matrix, -pivotZ);
        matrix = glm::translate(matrix, positionVec);
        matrix = glm::scale(matrix, scaleVec);
        return matrix;
    }

    void Transform::reset() {
        this->matrix = glm::mat4(1.0f);
        pivotX = glm::vec3(0.0f);
        pivotY = glm::vec3(0.0f);
        pivotZ = glm::vec3(0.0f);
        rotationVec = glm::vec3(0.0f);
        positionVec = glm::vec3(0.0f);
        scaleVec = glm::vec3(1.0f);
    }

    void Transform::destroy() {
        
    }
}