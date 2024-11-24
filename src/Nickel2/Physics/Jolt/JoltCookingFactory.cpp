#include <Nickel2/nkpch.hpp>
#include <Nickel2/Physics/Jolt/JoltCookingFactory.hpp>
#include <Nickel2/Physics/Jolt/JoltBinaryStream.hpp>
#include <Nickel2/Physics/Jolt/JoltShapes.hpp>
#include <Nickel2/Physics/Jolt/JoltUtils.hpp>

#include <Nickel2/Scene/Entity.hpp>

#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>

namespace Nickel2 {
    void JoltCookingFactory::Initialize() {}
    void JoltCookingFactory::Terminate() {}

    std::shared_ptr<ConvexMeshShape> JoltCookingFactory::CookConvexMesh(Entity* entity) {
        std::vector<MeshColliderData> colliderData;
        JPH::Array<JPH::Vec3> positions;

        for (const auto& [name, submesh] : entity->GetComponent<MeshComponent>()->GetSubmeshes()) {
            const std::vector<Vertex>& vertices = submesh->GetComponent<SubmeshComponent>()->GetVertices();

            for (uint32_t index : submesh->GetComponent<SubmeshComponent>()->GetIndices())
                positions.push_back(Utils::ToJoltVector(glm::vec3(submesh->GetComponent<TransformComponent>().GetLocalTransform() * glm::vec4(vertices[index].position, 1.0f))));
        }
        
        JPH::RefConst<JPH::ConvexHullShapeSettings> meshSettings = new JPH::ConvexHullShapeSettings(positions);
        JPH::Shape::ShapeResult result = meshSettings->Create();

        if (result.HasError())
            NK_CORE_ERROR_TAG("Physics", ("Failed to cook convex collider mesh, error: " + result.GetError()).c_str());

        JoltBinaryStreamWriter bufferWriter;
        JPH::RefConst<JPH::Shape> shape = result.Get();
        shape->SaveBinaryState(bufferWriter);

        MeshColliderData& data = colliderData.emplace_back();
        data.collider = bufferWriter.ToBuffer();
        return std::make_shared<JoltConvexMeshShape>(entity, colliderData);
    }

    std::shared_ptr<TriangleMeshShape> JoltCookingFactory::CookTriangleMesh(Entity* entity) {
        std::vector<MeshColliderData> colliderData;

        for (const auto& [name, submesh] : entity->GetComponent<MeshComponent>()->GetSubmeshes()) {
            JPH::VertexList vertexList;
            JPH::IndexedTriangleList triangleList;

            const std::vector<Vertex>& vertices = submesh->GetComponent<SubmeshComponent>()->GetVertices();
            const std::vector<uint32_t>& indices = submesh->GetComponent<SubmeshComponent>()->GetIndices();

            for (uint32_t index : indices)
                vertexList.push_back(JPH::Float3(vertices[index].position.x, vertices[index].position.y, vertices[index].position.z));

            for (uint32_t index = 0; index < indices.size(); index += 3)
                triangleList.push_back(JPH::IndexedTriangle(index, index + 1, index + 2, 0));

            JPH::RefConst<JPH::MeshShapeSettings> meshSettings = new JPH::MeshShapeSettings(vertexList, triangleList);
            JPH::Shape::ShapeResult result = meshSettings->Create();

            if (result.HasError()) {
                NK_CORE_ERROR_TAG("Physics", ("Failed to cook triangle collider mesh, error: " + result.GetError()).c_str());
                break;
            }

            JoltBinaryStreamWriter bufferWriter;
            JPH::RefConst<JPH::Shape> shape = result.Get();
            shape->SaveBinaryState(bufferWriter);

            MeshColliderData& data = colliderData.emplace_back();
            data.collider = bufferWriter.ToBuffer();
            data.transform = submesh->GetComponent<TransformComponent>().GetLocalTransform();
        } return std::make_shared<JoltTriangleMeshShape>(entity, colliderData);
    }
}