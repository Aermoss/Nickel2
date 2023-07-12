#include "mesh.hpp"

namespace nickel2 {
    const char* textureTypes[] = {
        "albedoMap", "roughnessMap", "metallicMap", "normalMap", "specularMap", "ambientMap"
    };

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

    Mesh::Mesh(std::vector <Vertex> vertices, std::vector <uint32_t> indices, std::vector <Texture> textures)
        : vertices(vertices), indices(indices), textures(textures) {
        setupMesh();
    }

    Mesh::~Mesh() {

    }

    void Mesh::render(Shader* shader) {
        glEnable(GL_DEPTH_TEST);
        shader->use();

        for (uint32_t i = 0; i < textures.size(); i++) {
            textures[i].bind();
            textures[i].texUnit(shader, textureTypes[i]);
        }

        vertexArray->bind();
        indexBuffer->bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        indexBuffer->unbind();
        vertexArray->unbind();

        for (uint32_t i = 0; i < textures.size(); i++) {
            textures[i].unbind();
        }

        shader->unuse();
        glDisable(GL_DEPTH_TEST);
    }

    void Mesh::destroy() {
        vertexBuffer->destroy();
        indexBuffer->destroy();
        vertexArray->destroy();
        delete vertexBuffer;
        delete indexBuffer;
        delete vertexArray;
    }
}