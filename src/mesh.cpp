#include <nickel2/mesh.hpp>

namespace nickel2 {
    void Mesh::setupMesh() {
        vertexArray = new VertexArray();
        vertexBuffer = new Buffer(GL_ARRAY_BUFFER);
        indexBuffer = new Buffer(GL_ELEMENT_ARRAY_BUFFER);

        vertexArray->bind();
        vertexBuffer->bind();
        vertexBuffer->bufferData(vertices.size() * sizeof(Vertex), vertices.data());
        indexBuffer->bind();
        indexBuffer->bufferData(indices.size() * sizeof(uint32_t), indices.data());
        vertexArray->defineAttrib(0, 3, sizeof(Vertex), (void*) offsetof(Vertex, position));
        vertexArray->defineAttrib(1, 2, sizeof(Vertex), (void*) offsetof(Vertex, texCoord));
        vertexArray->defineAttrib(2, 3, sizeof(Vertex), (void*) offsetof(Vertex, normal));
        vertexBuffer->unbind();
        indexBuffer->unbind();
        vertexArray->unbind();
    }

    Mesh::Mesh(std::vector <Vertex> vertices, std::vector <uint32_t> indices, Material& material, Transform* parent)
        : vertices(vertices), indices(indices), material(material) {

        transform = new Transform();

        if (parent != nullptr) {
            transform->parent = parent;
            parent->children.push_back(transform);
        }

        setupMesh();
    }

    Mesh::~Mesh() {

    }

    void Mesh::render(Shader* shader, bool useTexture) {
        glEnable(GL_DEPTH_TEST);
        shader->use();
        shader->setUniform3fv("albedoDefault", (float*) glm::value_ptr(material.albedo));
        shader->setUniform1f("roughnessDefault", material.roughness);
        shader->setUniform1f("metallicDefault", material.metallic);
        shader->setUniform1f("ambientDefault", 0.5f);
        glm::mat4 matrix = transform->getWorldMatrix();
        shader->setUniformMatrix3fv("modelMatrix", (float*) glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(matrix)))));
        shader->setUniformMatrix4fv("model", glm::value_ptr(matrix));

        std::vector <Texture*> textures = material.getTextures();

        for (uint32_t i = 0; i < (useTexture ? textures.size() : 0); i++) {
            if (textures[i] == nullptr) {
                shader->setUniform1i(textureTypesUniformNames[i], 0);
            } else {
                shader->setUniform1i(textureTypesUniformNames[i], 1);
                textures[i]->texUnit(shader, textureTypes[i]);
                textures[i]->bind();
            }
        }

        vertexArray->bind();
        indexBuffer->bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        indexBuffer->unbind();
        vertexArray->unbind();

        for (uint32_t i = 0; i < (useTexture ? textures.size() : 0); i++) {
            shader->setUniform1i(textureTypesUniformNames[i], 0);
            if (textures[i] == nullptr) continue;
            textures[i]->unbind();
        }

        shader->unuse();
        glDisable(GL_DEPTH_TEST);
    }

    void Mesh::destroy() {
        transform->destroy();
        delete transform;
        vertexBuffer->destroy();
        indexBuffer->destroy();
        vertexArray->destroy();
        delete vertexBuffer;
        delete indexBuffer;
        delete vertexArray;
    }
}