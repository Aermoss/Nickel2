#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/Mesh.hpp>
#include <Nickel2/Scene/Entity.hpp>

namespace Nickel2 {
    namespace Utils {
        glm::mat4 Mat4FromAIMatrix4x4(const aiMatrix4x4& matrix) {
            glm::mat4 result;
            result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
            result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
            result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
            result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
            return result;
        }
    }

    static const uint32_t flags = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace \
                                | aiProcess_JoinIdenticalVertices | aiProcess_LimitBoneWeights | aiProcess_ValidateDataStructure | aiProcess_GlobalScale;

    void Mesh::LoadMesh(std::string const& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, flags);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            Nickel2::Logger::Log(Nickel2::Logger::Level::Error, "Mesh", ("Failed to load model: \"" + path + "\".").c_str());
            return;
        } else {
            Nickel2::Logger::Log(Nickel2::Logger::Level::Info, "Mesh", ("Model loaded successfully: \"" + path + "\".").c_str());
        }

        directory = path.substr(0, path.find_last_of("/\\"));
        ProcessNode(scene->mRootNode, scene);
    }

    void Mesh::ProcessNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform) {
        glm::mat4 localTransform = Utils::Mat4FromAIMatrix4x4(node->mTransformation);
        glm::mat4 transform = parentTransform * localTransform;

        for (uint32_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            ProcessMesh(mesh, scene, transform);
        }

        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene, transform);
        }
    }

    void Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& parentTransform) {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

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

        aiString matName;
        aiColor4D albedo, ambient;
        float transparent, metallic = 0.0f, roughness = 0.0f;
        int32_t shadingMode; {
            aiGetMaterialString(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_NAME, &matName);
            aiGetMaterialColor(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_COLOR_DIFFUSE, &albedo);
            aiGetMaterialColor(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_COLOR_AMBIENT, &ambient);
            aiGetMaterialFloat(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_TRANSMISSION_FACTOR, &transparent);
            aiGetMaterialInteger(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_SHADING_MODEL, &shadingMode);

            if (shadingMode == aiShadingMode_PBR_BRDF) {
                aiGetMaterialFloat(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_METALLIC_FACTOR, &metallic);
                aiGetMaterialFloat(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_ROUGHNESS_FACTOR, &roughness);
            }
        }

        Material material = {
            .name = std::string(matName.C_Str()), .shadingMode = shadingMode,
            .albedo = glm::vec4(albedo.r, albedo.g, albedo.b, albedo.a),
            .ambient = ambient.r, .roughness = roughness,
            .metallic = metallic, .transparent = transparent,
            .textureScale = 1, .rotateTexture = false
        };

        auto GetMaterialTexture = [&](aiMaterial* mat, aiTextureType type, uint32_t slot) -> std::string {
            aiString str; {
                if (mat->GetTextureCount(type) == 0) return "";
                mat->GetTexture(type, 0, &str);
            }

            std::string filePath = directory + "/";
            std::string filePathRaw = str.C_Str();
            return filePath + filePathRaw.substr(filePathRaw.find_last_of("/\\") + 1);
        };

        std::vector<std::string> texturePaths = {
            GetMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, 0),
            GetMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE_ROUGHNESS, 1),
            GetMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_METALNESS, 2),
            GetMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_NORMALS, 3),
            GetMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR, 4),
            GetMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_AMBIENT, 5)
        };

        if (texturePaths[1] == texturePaths[2])
            texturePaths[2] = "";

        for (uint32_t i = 0; i < texturePaths.size(); i++) {
            if (texturePaths[i].empty())
                continue;

            if (textures.find(texturePaths[i]) == textures.end())
                textures[texturePaths[i]] = Texture2D::Create(texturePaths[i]);

            switch (i) {
                case 0: material.albedoMap = textures[texturePaths[i]]; break;
                case 1: material.roughnessMap = textures[texturePaths[i]]; break;
                case 2: material.metallicMap = textures[texturePaths[i]]; break;
                case 3: material.normalMap = textures[texturePaths[i]]; break;
                case 4: material.specularMap = textures[texturePaths[i]]; break;
                case 5: material.ambientMap = textures[texturePaths[i]]; break;
                default: break;
            }
        }

        std::string name = mesh->mName.C_Str();

        if (submeshes.find(name) != submeshes.end())
            name += "_" + std::to_string(submeshes.size());

        std::shared_ptr<Entity> submesh = std::make_shared<Entity>(entity->GetScene(), name);
        submesh->AddComponent<SubmeshComponent>(vertices, indices, material);
        submesh->GetComponent<TransformComponent>().DecomposeTransform(parentTransform);
        submesh->GetComponent<RelationshipComponent>().SetParent(entity);
        submeshes[name] = submesh;
    }

    Mesh::Mesh(Entity* entity, std::string const& path) : entity(entity) {
        this->LoadMesh(path);
    }

    void Mesh::Render(std::shared_ptr<Shader> shader, bool useTexture, bool skipTransparent) {
        for (auto& [key, value]: submeshes) {
            if (value->GetComponent<SubmeshComponent>()->material.albedo.w != 1.0f && skipTransparent) continue;
            value->GetComponent<SubmeshComponent>()->Render(shader, useTexture);
        }
    }

    void Mesh::Destroy() {
        textures.clear();
        submeshes.clear();
    }
}