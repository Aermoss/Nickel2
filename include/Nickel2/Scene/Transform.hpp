#pragma once

#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../Core/Context.hpp"
#include "../Core/Logger.hpp"

namespace Nickel2 {
    class Transform {
        private:
            uint64_t id;
            glm::mat4 matrix;
            glm::vec3 translation, _scale;
            glm::quat rotation;
            bool dirtyMatrix, dirtyDOF;

            uint64_t getUniqueChildID();
        
        public:
            Transform* parent;
            std::unordered_map <uint64_t, Transform*> children;

            Transform();
            ~Transform();

            void setParent(Transform* parent);
            uint64_t addChild(Transform* child);
            void removeChild(uint64_t id);

            void setTranslation(const glm::vec3& translation);
            void setScale(const glm::vec3& scale);
            void setRotation(const glm::vec3& rotation);
            void setRotationQuat(const glm::quat& rotation);
            
            void translate(const glm::vec3& vector);
            void scale(const glm::vec3& vector);
            void rotate(const glm::vec3& vector);

            glm::vec3 getTranslation();
            glm::vec3 getWorldTranslation();
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