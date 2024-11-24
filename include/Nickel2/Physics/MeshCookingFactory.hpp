#pragma once

#include <Nickel2/Core/Base.hpp>
#include <Nickel2/Core/Buffer.hpp>
#include <Nickel2/Physics/PhysicsTypes.hpp>
#include <Nickel2/Physics/PhysicsShapes.hpp>
#include <Nickel2/Renderer/Mesh.hpp>

namespace Nickel2 {
    struct MeshColliderData {
        Buffer collider;
        glm::mat4 transform;
    };

    class MeshCookingFactory {
        public:
            virtual ~MeshCookingFactory() = default;

            virtual void Initialize() = 0;
            virtual void Terminate() = 0;

            virtual std::shared_ptr<ConvexMeshShape> CookConvexMesh(Entity* entity) = 0;
            virtual std::shared_ptr<TriangleMeshShape> CookTriangleMesh(Entity* entity) = 0;
    };
}