#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>

#include <glm/glm.hpp>

namespace Nickel2 {
    enum class ShaderStage {
        None = 0, Vertex, Fragment, Compute, Geometry, TessellationControl, TessellationEvaluation
    };

    class Shader {
        public:
            virtual ~Shader() = default;

            virtual void Bind() const = 0;
            virtual void Unbind() const = 0;

            virtual void SetIntArray(const std::string& name, const int32_t* values, uint32_t count) = 0;
            virtual void SetIntArray2(const std::string& name, const glm::ivec2* values, uint32_t count) = 0;
            virtual void SetIntArray3(const std::string& name, const glm::ivec3* values, uint32_t count) = 0;
            virtual void SetIntArray4(const std::string& name, const glm::ivec4* values, uint32_t count) = 0;

            virtual void SetInt(const std::string& name, int32_t value) = 0;
            virtual void SetInt2(const std::string& name, const glm::ivec2& value) = 0;
            virtual void SetInt3(const std::string& name, const glm::ivec3& value) = 0;
            virtual void SetInt4(const std::string& name, const glm::ivec4& value) = 0;

            virtual void SetFloatArray(const std::string& name, const float* values, uint32_t count) = 0;
            virtual void SetFloatArray2(const std::string& name, const glm::vec2* values, uint32_t count) = 0;
            virtual void SetFloatArray3(const std::string& name, const glm::vec3* values, uint32_t count) = 0;
            virtual void SetFloatArray4(const std::string& name, const glm::vec4* values, uint32_t count) = 0;

            virtual void SetFloat(const std::string& name, float value) = 0;
            virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
            virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
            virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;

            virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;
            virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

            virtual void SetMatArray3(const std::string& name, const glm::mat3* values, uint32_t count) = 0;
            virtual void SetMatArray4(const std::string& name, const glm::mat4* values, uint32_t count) = 0;

            virtual int32_t GetUniformLocation(const std::string& name) = 0;
            virtual const std::string& GetName() const = 0;

            static std::shared_ptr<Shader> Create(const std::string& name, const std::unordered_map<ShaderStage, std::string>& sources);
    };

    class ShaderLibrary {
        private:
            std::unordered_map<std::string, std::unordered_map<ShaderStage, std::string>> shaderPaths;
            std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;

        public:
            const std::shared_ptr<Shader>& Load(const std::string& name, const std::unordered_map<ShaderStage, std::string>& paths);
            void Free(const std::shared_ptr<Shader>& shader);
            void Free(const std::string& name);
            
            void Add(const std::shared_ptr<Shader>& shader);
            void Remove(const std::shared_ptr<Shader>& shader);
            void Remove(const std::string& name);

            void Set(const std::string& name, const std::shared_ptr<Shader>& shader);
            const std::shared_ptr<Shader>& Get(const std::string& name) const;

            bool Exists(const std::string& name) const;
            bool Exists(const std::shared_ptr<Shader>& shader) const;
            
            void Reload(const std::string& name);
            void Reload(const std::shared_ptr<Shader>& shader);
            void Reload();
    };
}