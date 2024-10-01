#include <Nickel2/nkpch.hpp>
#include <Nickel2/Math/Math.hpp>

namespace Nickel2::Math {
    glm::vec3 Scale(const glm::vec3& v, float desiredLength) {
        return v * desiredLength / length(v);
    }

    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale) {
        using T = float;
        glm::mat4 localMatrix(transform);

        if (glm::epsilonEqual(localMatrix[3][3], static_cast<float>(0), glm::epsilon<T>()))
            return false;

        if (glm::epsilonNotEqual(localMatrix[0][3], static_cast<T>(0), glm::epsilon<T>())
         || glm::epsilonNotEqual(localMatrix[1][3], static_cast<T>(0), glm::epsilon<T>())
         || glm::epsilonNotEqual(localMatrix[2][3], static_cast<T>(0), glm::epsilon<T>())) {
            localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<T>(0);
            localMatrix[3][3] = static_cast<T>(1);
        }

        translation = glm::vec3(localMatrix[3]);
        localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

        glm::vec3 row[3];

        for (glm::length_t i = 0; i < 3; ++i)
            for (glm::length_t j = 0; j < 3; ++j)
                row[i][j] = localMatrix[i][j];

        scale.x = glm::length(row[0]);
        row[0] = glm::detail::scale(row[0], static_cast<T>(1));
        scale.y = glm::length(row[1]);
        row[1] = glm::detail::scale(row[1], static_cast<T>(1));
        scale.z = glm::length(row[2]);
        row[2] = glm::detail::scale(row[2], static_cast<T>(1));

        rotation.y = glm::asin(-row[0][2]);

        if (glm::cos(rotation.y) != 0) {
            rotation.x = atan2(row[1][2], row[2][2]);
            rotation.z = atan2(row[0][1], row[0][0]);
        } else {
            rotation.x = atan2(-row[2][0], row[1][1]);
            rotation.z = 0;
        } return true;
    }

    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::quat& rotation, glm::vec3& scale) {
        using T = float;
        glm::mat4 localMatrix(transform);

        if (glm::epsilonEqual(localMatrix[3][3], static_cast<T>(0), glm::epsilon<T>()))
            return false;

        NK_CORE_ASSERT(glm::epsilonEqual(localMatrix[3][3], static_cast<T>(1), static_cast<T>(0.00001)));

        NK_CORE_ASSERT(
            glm::epsilonEqual(localMatrix[0][3], static_cast<T>(0), glm::epsilon<T>()) &&
            glm::epsilonEqual(localMatrix[1][3], static_cast<T>(0), glm::epsilon<T>()) &&
            glm::epsilonEqual(localMatrix[2][3], static_cast<T>(0), glm::epsilon<T>())
        );

        translation = glm::vec3(localMatrix[3]);
        localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

        glm::vec3 row[3];

        for (glm::length_t i = 0; i < 3; ++i)
            for (glm::length_t j = 0; j < 3; ++j)
                row[i][j] = localMatrix[i][j];

        scale.x = length(row[0]);
        row[0] = Scale(row[0], static_cast<T>(1));

        scale.y = length(row[1]);
        row[1] = Scale(row[1], static_cast<T>(1));

        scale.z = length(row[2]);
        row[2] = Scale(row[2], static_cast<T>(1));

        int i, j, k = 0;
        T root, trace = row[0].x + row[1].y + row[2].z;

        if (trace > static_cast<T>(0)) {
            root = glm::sqrt(trace + static_cast<T>(1));
            rotation.w = static_cast<T>(0.5) * root;
            root = static_cast<T>(0.5) / root;
            rotation.x = root * (row[1].z - row[2].y);
            rotation.y = root * (row[2].x - row[0].z);
            rotation.z = root * (row[0].y - row[1].x);
        } else {
            static int next[3] = { 1, 2, 0 };
            i = 0;
            if (row[1].y > row[0].x) i = 1;
            if (row[2].z > row[i][i]) i = 2;
            j = next[i];
            k = next[j];

            root = glm::sqrt(row[i][i] - row[j][j] - row[k][k] + static_cast<T>(1.0));

            rotation[i] = static_cast<T>(0.5) * root;
            root = static_cast<T>(0.5) / root;
            rotation[j] = root * (row[i][j] + row[j][i]);
            rotation[k] = root * (row[i][k] + row[k][i]);
            rotation.w = root * (row[j][k] - row[k][j]);
        } return true;
    }
}