#pragma once

#include "../Shader.hpp"
#include "../../Core/Utils.hpp"

#include <glm/glm.hpp>

namespace Nickel2 {
    class OpenGLShader : public Shader {
        private:
            uint32_t id = 0;
            std::string name;

            void CreateProgram(const std::unordered_map<ShaderStage, std::string>& sources);
            void DestroyProgram();

        public:
            OpenGLShader(const std::string& name, const std::unordered_map<ShaderStage, std::string>& sources);
            virtual ~OpenGLShader();

            virtual void Bind() const override;
            virtual void Unbind() const override;

            virtual void SetIntArray(const std::string& name, const int32_t* values, uint32_t count) override;
            virtual void SetIntArray2(const std::string& name, const glm::ivec2* values, uint32_t count) override;
            virtual void SetIntArray3(const std::string& name, const glm::ivec3* values, uint32_t count) override;
            virtual void SetIntArray4(const std::string& name, const glm::ivec4* values, uint32_t count) override;

            virtual void SetInt(const std::string& name, int32_t value) override;
            virtual void SetInt2(const std::string& name, const glm::ivec2& value) override;
            virtual void SetInt3(const std::string& name, const glm::ivec3& value) override;
            virtual void SetInt4(const std::string& name, const glm::ivec4& value) override;

            virtual void SetFloatArray(const std::string& name, const float* values, uint32_t count) override;
            virtual void SetFloatArray2(const std::string& name, const glm::vec2* values, uint32_t count) override;
            virtual void SetFloatArray3(const std::string& name, const glm::vec3* values, uint32_t count) override;
            virtual void SetFloatArray4(const std::string& name, const glm::vec4* values, uint32_t count) override;

            virtual void SetFloat(const std::string& name, float value) override;
            virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
            virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
            virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;

            virtual void SetMat3(const std::string& name, const glm::mat3& value) override;
            virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

            virtual void SetMatArray3(const std::string& name, const glm::mat3* values, uint32_t count) override;
            virtual void SetMatArray4(const std::string& name, const glm::mat4* values, uint32_t count) override;

            virtual int32_t GetUniformLocation(const std::string& name) override;
            virtual const std::string& GetName() const override { return name; }
    };
}