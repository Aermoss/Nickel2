#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/Submesh.hpp>
#include <Nickel2/Renderer/Renderer.hpp>
#include <Nickel2/Scene/Entity.hpp>

namespace Nickel2 {
    const char* textureTypes[] = {
        "albedoMap", "roughnessMap", "metallicMap", "normalMap", "specularMap", "ambientMap"
    };

    const char* textureTypesUniformNames[] = {
        "useAlbedoMap", "useRoughnessMap", "useMetallicMap", "useNormalMap", "useSpecularMap", "useAmbientMap"
    };

    Submesh::Submesh(Entity* entity, std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material material) : entity(entity), vertices(vertices), indices(indices), material(material) {
        vertexArray = VertexArray::Create();
        vertexBuffer = VertexBuffer::Create((float*) vertices.data(), vertices.size() * sizeof(Vertex));
        indexBuffer = IndexBuffer::Create(indices.data(), indices.size());

        vertexBuffer->SetLayout({
            { Nickel2::DataType::Float3, "position" },
            { Nickel2::DataType::Float2, "texCoord" },
            { Nickel2::DataType::Float3, "color" }
        });

        vertexArray->AddVertexBuffer(vertexBuffer);
        vertexArray->AddIndexBuffer(indexBuffer);
    }

    void Submesh::Render(std::shared_ptr<Shader> shader, bool useTexture) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, material.albedo.a < 1.0f ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        shader->Bind();
        shader->SetFloat4("albedoDefault", material.albedo);
        shader->SetFloat("roughnessDefault", material.roughness);
        shader->SetFloat("metallicDefault", material.metallic);
        shader->SetFloat("ambientDefault", material.ambient);
        shader->SetInt("rotateTexture", material.rotateTexture);
        shader->SetInt("textureScale", material.textureScale);
        glm::mat4 matrix = entity->GetComponent<TransformComponent>().GetWorldTransform();
        shader->SetMat3("inverseModel", glm::transpose(glm::inverse(glm::mat3(matrix))));
        shader->SetMat4("model", matrix);

        std::vector<std::shared_ptr<Texture>> textures = material.GetTextures();

        for (uint32_t i = 0; i < (useTexture ? textures.size() : 0); i++) {
            if (textures[i] != nullptr) {
                shader->SetInt(textureTypesUniformNames[i], 1);
                shader->SetInt(textureTypes[i], i);
                textures[i]->Bind(i);
            } else {
                shader->SetInt(textureTypesUniformNames[i], 0);
            }
        }

        RenderCommand::DrawIndexed(vertexArray);
        // Renderer::Submit(shader, vertexArray);
    }

    void Submesh::Destroy() {
        vertexBuffer = nullptr, indexBuffer = nullptr, vertexArray = nullptr;
    }
}