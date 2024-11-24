#pragma once

#include <Nickel2/Physics/MeshCookingFactory.hpp>

namespace Nickel2 {
	class JoltCookingFactory : public MeshCookingFactory {
        public:
            virtual void Initialize() override;
            virtual void Terminate() override;

            virtual std::shared_ptr<ConvexMeshShape> CookConvexMesh(Entity* entity) override;
            virtual std::shared_ptr<TriangleMeshShape> CookTriangleMesh(Entity* entity) override;
	};
}