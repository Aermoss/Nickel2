#pragma once

#include <iostream>
#include <unordered_map>
#include <cstdint>
#include <string>

#include <glad/glad.h>

#include "../Core/Context.hpp"
#include "../Core/Logger.hpp"

namespace Nickel2 {
    class Shader {
        private:
            uint32_t id;
            uint32_t compileShader(uint32_t type, const char* source);
            uint32_t createProgram(const std::string& computeSource);
            uint32_t createProgram(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource = "", const std::string& tessCtrlSource = "", const std::string& tessEvalSource = "");

        public:
            Shader(const std::string& computeSource);
            Shader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource = "", const std::string& tessCtrlSource = "", const std::string& tessEvalSource = "");
            ~Shader();

            void use();
            void unuse();
            void reload(const std::string& computeSource);
            void reload(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource = "", const std::string& tessCtrlSource = "", const std::string& tessEvalSource = "");
            void destroy();
            int32_t getUniformLocation(const std::string& name);
            void setUniform1i(const std::string& name, int32_t x);
            void setUniform1ui(const std::string& name, uint32_t x);
            void setUniform2i(const std::string& name, int32_t x, int32_t y);
            void setUniform2ui(const std::string& name, uint32_t x, uint32_t y);
            void setUniform3i(const std::string& name, int32_t x, int32_t y, int32_t z);
            void setUniform3ui(const std::string& name, uint32_t x, uint32_t y, uint32_t z);
            void setUniform4i(const std::string& name, int32_t x, int32_t y, int32_t z, int32_t w);
            void setUniform4ui(const std::string& name, uint32_t x, uint32_t y, uint32_t z, uint32_t w);
            void setUniform1iv(const std::string& name, int32_t* value, int32_t count = 1);
            void setUniform1uiv(const std::string& name, uint32_t* value, int32_t count = 1);
            void setUniform2iv(const std::string& name, int32_t* value, int32_t count = 1);
            void setUniform2uiv(const std::string& name, uint32_t* value, int32_t count = 1);
            void setUniform3iv(const std::string& name, int32_t* value, int32_t count = 1);
            void setUniform3uiv(const std::string& name, uint32_t* value, int32_t count = 1);
            void setUniform4iv(const std::string& name, int32_t* value, int32_t count = 1);
            void setUniform4uiv(const std::string& name, uint32_t* value, int32_t count = 1);
            void setUniform1f(const std::string& name, float x);
            void setUniform2f(const std::string& name, float x, float y);
            void setUniform3f(const std::string& name, float x, float y, float z);
            void setUniform4f(const std::string& name, float x, float y, float z, float w);
            void setUniform1fv(const std::string& name, float* value, int32_t count = 1);
            void setUniform2fv(const std::string& name, float* value, int32_t count = 1);
            void setUniform3fv(const std::string& name, float* value, int32_t count = 1);
            void setUniform4fv(const std::string& name, float* value, int32_t count = 1);
            void setUniform1d(const std::string& name, double x);
            void setUniform2d(const std::string& name, double x, double y);
            void setUniform3d(const std::string& name, double x, double y, double z);
            void setUniform4d(const std::string& name, double x, double y, double z, double w);
            void setUniform1dv(const std::string& name, double* value, int32_t count = 1);
            void setUniform2dv(const std::string& name, double* value, int32_t count = 1);
            void setUniform3dv(const std::string& name, double* value, int32_t count = 1);
            void setUniform4dv(const std::string& name, double* value, int32_t count = 1);
            void setUniformMatrix3fv(const std::string& name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix4fv(const std::string& name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix2x3fv(const std::string& name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix2x4fv(const std::string& name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix3x2fv(const std::string& name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix3x4fv(const std::string& name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix4x2fv(const std::string& name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix4x3fv(const std::string& name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix3dv(const std::string& name, double* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix4dv(const std::string& name, double* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix2x3dv(const std::string& name, double* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix2x4dv(const std::string& name, double* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix3x2dv(const std::string& name, double* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix3x4dv(const std::string& name, double* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix4x2dv(const std::string& name, double* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix4x3dv(const std::string& name, double* value, int32_t count = 1, bool transpose = false);
    };
}