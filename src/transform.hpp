#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace nickel2 {
    class Transform {
        private:
            glm::mat4 matrix;
            glm::vec3 rotationVec, positionVec, scaleVec;
        
        public:
            glm::vec3 pivotX, pivotY, pivotZ;
            
            Transform();
            ~Transform();

            void setMatrix(glm::mat4 matrix);
            void setRotation(glm::vec3 rotation);
            void setPosition(glm::vec3 position);
            void setScale(glm::vec3 scale);
            glm::vec3 getRotation();
            glm::vec3 getPosition();
            glm::vec3 getScale();
            void rotate(glm::vec3 value);
            void translate(glm::vec3 value);
            void scale(glm::vec3 value);
            glm::mat4 get();
            void reset();
            void destroy();
    };
}