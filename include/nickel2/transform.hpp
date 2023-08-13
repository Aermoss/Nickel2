#pragma once

#include <unordered_map>

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
            uint32_t id;
            glm::mat4 matrix;
            glm::vec3 position, _scale;
            glm::quat rotation;
            bool dirtyMatrix, dirtyDOF;

            uint32_t getUniqueChildID();
        
        public:
            Transform* parent;
            std::unordered_map <uint32_t, Transform*> children;

            Transform();
            ~Transform();

            void setParent(Transform* parent);
            uint32_t addChild(Transform* child);
            void removeChild(uint32_t id);

            void setPosition(const glm::vec3& position);
            void setScale(const glm::vec3& scale);
            void setRotation(const glm::vec3& rotation);
            void setRotationQuat(const glm::quat& rotation);
            
            void translate(const glm::vec3& vector);
            void scale(const glm::vec3& vector);
            void rotate(const glm::vec3& vector);

            glm::vec3 getPosition();
            glm::vec3 getWorldPosition();
            glm::vec3 getScale();
            glm::vec3 getRotation();
            glm::quat getRotationQuat();
            glm::quat getWorldRotationQuat();

            void overrideMatrix(glm::mat4 matrix);
            void updateWorldMatrix(bool dirtyParent = false);
            glm::mat4 getLocalMatrix() const;
            glm::mat4 getParentMatrix() const;
            glm::mat4 getWorldMatrix() const;

            void reset();
            void destroy();
    };
}