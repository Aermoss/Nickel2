#include "model.hpp"

namespace nickel2 {
    void Model::loadModel(std::string const& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "Assimp Error" << std::endl;
            return;
        }

        directory = path.substr(0, path.find_last_of("/\\"));
        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode* node, const aiScene* scene) {
        for (uint32_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector <Vertex> vertices;
        std::vector <uint32_t> indices;
        std::vector <Texture> textures;

        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
            
            if (mesh->HasNormals())
                vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

            if (mesh->mTextureCoords[0])
                vertex.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

            vertices.push_back(vertex);
        }

        for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];

            for (uint32_t j = 0; j < face.mNumIndices; j++) 
                indices.push_back(face.mIndices[j]);
        }

        Material material;
        material.albedoMap = getMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE);
        material.roughnessMap = nullptr; // getMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], 0);
        material.metallicMap = nullptr; // getMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], 0);
        material.normalMap = getMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_NORMALS);
        material.specularMap = getMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR);
        material.ambientMap = getMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_AMBIENT);
        return Mesh(vertices, indices, material);
    }

    Texture* Model::getMaterialTexture(aiMaterial* mat, aiTextureType type) {
        Texture* texture = nullptr;

        if (mat->GetTextureCount(type) != 0) {
            aiString str;
            mat->GetTexture(type, 0, &str);
            std::string filePath = directory + "/";
            std::string filePathRaw = str.C_Str();
            filePath += filePathRaw.substr(filePathRaw.find_last_of("/\\") + 1);
            filePath = filePath.substr(0, filePath.find_last_of('.')) + ".png"; // TODO: fix file not found error
            std::cout << filePath << std::endl;
            texture = new Texture(filePath.c_str(), 0);
        } return texture;
    }

    Model::Model(std::string const& path) {
        loadModel(path);
    }

    Model::~Model() {

    }

    void Model::render(Shader* shader) {
        for (uint32_t i = 0; i < meshes.size(); i++) {
            meshes[i].render(shader);
        }
    }

    void Model::destroy() {
        for (uint32_t i = 0; i < meshes.size(); i++) {
            meshes[i].destroy();
        }
    }
}