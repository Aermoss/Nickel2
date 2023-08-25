#include <nickel2/model.hpp>

namespace nickel2 {
    void Model::loadModel(std::string const& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            context->logger->log(NICKEL2_ERROR, ("failed to model: \"" + path + "\".").c_str());
            return;
        } else {
            context->logger->log(NICKEL2_INFO, ("model successfully loaded: \"" + path + "\".").c_str());
        }

        directory = path.substr(0, path.find_last_of("/\\"));
        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode* node, const aiScene* scene) {
        for (uint32_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }

        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    void Model::processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector <Vertex> vertices;
        std::vector <uint32_t> indices;

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

        aiString name;
        aiGetMaterialString(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_NAME, &name);

        aiColor4D albedo, ambient;
        aiGetMaterialColor(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_COLOR_DIFFUSE, &albedo);
        aiGetMaterialColor(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_COLOR_AMBIENT, &ambient);

        float transparent, metallic = 0.0f, roughness = 0.0f;
        aiGetMaterialFloat(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_TRANSMISSION_FACTOR, &transparent);

        int32_t shadingMode;
        aiGetMaterialInteger(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_SHADING_MODEL, &shadingMode);

        if (shadingMode == aiShadingMode_PBR_BRDF) {
            aiGetMaterialFloat(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_METALLIC_FACTOR, &metallic);
            aiGetMaterialFloat(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_ROUGHNESS_FACTOR, &roughness);
        }

        Material material;
        material.name = std::string(name.C_Str());
        material.shadingMode = shadingMode;
        material.albedo = glm::vec4(albedo.r, albedo.g, albedo.b, albedo.a);
        material.albedoMap = getMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, 0);
        material.roughnessMap = getMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE_ROUGHNESS, 1);
        material.metallicMap = getMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_METALNESS, 2);
        material.normalMap = getMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_NORMALS, 3);
        material.specularMap = getMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR, 4);
        material.ambientMap = getMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_AMBIENT, 5);
        material.ambient = ambient.r, material.roughness = roughness;
        material.metallic = metallic, material.transparent = transparent;
        material.rotateTexture = false;
        material.textureScale = 1;

        uint32_t id = 0;
        while (meshes.find((std::to_string(id) + "_" + mesh->mName.C_Str()).c_str()) != meshes.end()) id++;
        meshes[(std::to_string(id) + "_" + mesh->mName.C_Str()).c_str()] = new Mesh(vertices, indices, material, transform);
    }

    Texture* Model::getMaterialTexture(aiMaterial* mat, aiTextureType type, uint32_t slot) {
        Texture* texture = nullptr;

        if (mat->GetTextureCount(type) != 0) {
            aiString str;
            mat->GetTexture(type, 0, &str);
            std::string filePath = directory + "/";
            std::string filePathRaw = str.C_Str();
            filePath += filePathRaw.substr(filePathRaw.find_last_of("/\\") + 1);
            // filePath = filePath.substr(0, filePath.find_last_of('.')) + ".png";

            if (loadedTextures.find(filePath) != loadedTextures.end()) {
                texture = loadedTextures[filePath];
            } else {
                texture = new Texture(filePath.c_str(), slot);
                loadedTextures[filePath] = texture;
            }
        } return texture;
    }

    Model::Model(std::string const& path) {
        transform = new Transform();
        loadModel(path);
    }

    Model::~Model() {

    }

    std::unordered_map <std::string, Mesh*>& Model::getMeshes() {
        return meshes;
    }

    std::unordered_map <std::string, Texture*>& Model::getTextures() {
        return loadedTextures;
    }

    const std::string& Model::getDirectory() {
        return directory;
    }

    void Model::render(Shader* shader, bool useTexture) {
        transform->updateWorldMatrix(false);

        for (const auto &[key, value]: meshes) {
            meshes[key]->render(shader, useTexture);
        }
    }

    void Model::destroy() {
        transform->destroy();
        delete transform;

        for (const auto &[key, value]: loadedTextures) {
            loadedTextures[key]->destroy();
            delete loadedTextures[key];
        } loadedTextures.clear();

        for (const auto &[key, value]: meshes) {
            meshes[key]->destroy();
            delete meshes[key];
        } meshes.clear();
    }
}