#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtx/quaternion.hpp>

#include "context.hpp"
#include "logger.hpp"

namespace nickel2 {
    class Transform {
        private:
            glm::mat4 matrix;
            glm::vec3 position, scale;
            glm::quat rotation;
            bool dirtyMatrix, dirtyDOF;
        
        public:
            Transform* parent;
            std::vector <Transform*> children;

            Transform();
            ~Transform();

            void setPosition(const glm::vec3& position);
            void setScale(const glm::vec3& scale);
            void setRotation(const glm::vec3& rotation);
            void setRotationQuat(const glm::quat& rotation);
            void rotateBy(const glm::vec3& rotation);

            glm::vec3 getPosition();
            glm::vec3 getWorldPosition();
            glm::vec3 getScale();
            glm::vec3 getRotation();
            glm::quat getRotationQuat();
            glm::quat getWorldRotationQuat();

            void overrideMatrix(glm::mat4 matrix);
            void updateWorldMatrix(bool dirtyParent);
            glm::mat4 getLocalMatrix() const;
            glm::mat4 getParentMatrix() const;
            glm::mat4 getWorldMatrix() const;

            void reset();
            void destroy();
    };
}