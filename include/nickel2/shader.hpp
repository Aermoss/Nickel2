#pragma once

#include <iostream>
#include <string>
#include <stdint.h>

#include <glad/glad.h>

#include "context.hpp"
#include "logger.hpp"

namespace nickel2 {
    class Shader {
        private:
            uint32_t id;
            uint32_t compileShader(uint32_t type, const char* source);
            uint32_t createProgram(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource = "");

        public:
            Shader(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource = "");
            ~Shader();

            void use();
            void unuse();
            void reload(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource = "");
            void destroy();
            int32_t getUniformLocation(const char* name);
            void setUniform1i(const char* name, int32_t x);
            void setUniform1ui(const char* name, uint32_t x);
            void setUniform2i(const char* name, int32_t x, int32_t y);
            void setUniform2ui(const char* name, uint32_t x, uint32_t y);
            void setUniform3i(const char* name, int32_t x, int32_t y, int32_t z);
            void setUniform3ui(const char* name, uint32_t x, uint32_t y, uint32_t z);
            void setUniform4i(const char* name, int32_t x, int32_t y, int32_t z, int32_t w);
            void setUniform4ui(const char* name, uint32_t x, uint32_t y, uint32_t z, uint32_t w);
            void setUniform1iv(const char* name, int32_t* value, int32_t count = 1);
            void setUniform1uiv(const char* name, uint32_t* value, int32_t count = 1);
            void setUniform2iv(const char* name, int32_t* value, int32_t count = 1);
            void setUniform2uiv(const char* name, uint32_t* value, int32_t count = 1);
            void setUniform3iv(const char* name, int32_t* value, int32_t count = 1);
            void setUniform3uiv(const char* name, uint32_t* value, int32_t count = 1);
            void setUniform4iv(const char* name, int32_t* value, int32_t count = 1);
            void setUniform4uiv(const char* name, uint32_t* value, int32_t count = 1);
            void setUniform1f(const char* name, float x);
            void setUniform2f(const char* name, float x, float y);
            void setUniform3f(const char* name, float x, float y, float z);
            void setUniform4f(const char* name, float x, float y, float z, float w);
            void setUniform1fv(const char* name, float* value, int32_t count = 1);
            void setUniform2fv(const char* name, float* value, int32_t count = 1);
            void setUniform3fv(const char* name, float* value, int32_t count = 1);
            void setUniform4fv(const char* name, float* value, int32_t count = 1);
            void setUniform1d(const char* name, double x);
            void setUniform2d(const char* name, double x, double y);
            void setUniform3d(const char* name, double x, double y, double z);
            void setUniform4d(const char* name, double x, double y, double z, double w);
            void setUniform1dv(const char* name, double* value, int32_t count = 1);
            void setUniform2dv(const char* name, double* value, int32_t count = 1);
            void setUniform3dv(const char* name, double* value, int32_t count = 1);
            void setUniform4dv(const char* name, double* value, int32_t count = 1);
            void setUniformMatrix3fv(const char* name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix4fv(const char* name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix2x3fv(const char* name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix2x4fv(const char* name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix3x2fv(const char* name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix3x4fv(const char* name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix4x2fv(const char* name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix4x3fv(const char* name, float* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix3dv(const char* name, double* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix4dv(const char* name, double* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix2x3dv(const char* name, double* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix2x4dv(const char* name, double* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix3x2dv(const char* name, double* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix3x4dv(const char* name, double* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix4x2dv(const char* name, double* value, int32_t count = 1, bool transpose = false);
            void setUniformMatrix4x3dv(const char* name, double* value, int32_t count = 1, bool transpose = false);
    };
}