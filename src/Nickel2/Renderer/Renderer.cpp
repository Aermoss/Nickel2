#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/Renderer.hpp>

namespace Nickel2 {
    VertexArray* sphereVertexArray = nullptr;
    Buffer* sphereVertexBuffer = nullptr;
    Buffer* sphereIndexBuffer = nullptr;
    uint32_t sphereIndexCount;

    void renderSphere() {
        if (sphereVertexArray == nullptr) {
            sphereVertexArray = new VertexArray();
            sphereVertexBuffer = new Buffer(NICKEL2_ARRAY_BUFFER);
            sphereIndexBuffer = new Buffer(NICKEL2_ELEMENT_ARRAY_BUFFER);

            std::vector <glm::vec3> positions;
            std::vector <glm::vec2> uv;
            std::vector <glm::vec3> normals;
            std::vector <uint32_t> indices;

            const uint32_t X_SEGMENTS = 64;
            const uint32_t Y_SEGMENTS = 64;
            const float PI = 3.14159265359f;

            for (uint32_t x = 0; x <= X_SEGMENTS; ++x) {
                for (uint32_t y = 0; y <= Y_SEGMENTS; ++y) {
                    float xSegment = (float)x / (float)X_SEGMENTS;
                    float ySegment = (float)y / (float)Y_SEGMENTS;
                    float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                    float yPos = std::cos(ySegment * PI);
                    float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                    positions.push_back(glm::vec3(xPos, yPos, zPos));
                    uv.push_back(glm::vec2(xSegment, ySegment));
                    normals.push_back(glm::vec3(xPos, yPos, zPos));
                }
            }

            bool oddRow = false;

            for (uint32_t y = 0; y < Y_SEGMENTS; ++y) {
                if (!oddRow) {
                    for (uint32_t x = 0; x <= X_SEGMENTS; ++x) {
                        indices.push_back(y * (X_SEGMENTS + 1) + x);
                        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    }
                } else {
                    for (int x = X_SEGMENTS; x >= 0; --x) {
                        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                        indices.push_back(y * (X_SEGMENTS + 1) + x);
                    }
                }

                oddRow = !oddRow;
            }

            sphereIndexCount = static_cast<uint32_t>(indices.size());
            std::vector <float> data;

            for (uint32_t i = 0; i < positions.size(); ++i) {
                data.push_back(positions[i].x);
                data.push_back(positions[i].y);
                data.push_back(positions[i].z);

                if (normals.size() > 0) {
                    data.push_back(normals[i].x);
                    data.push_back(normals[i].y);
                    data.push_back(normals[i].z);
                }

                if (uv.size() > 0) {
                    data.push_back(uv[i].x);
                    data.push_back(uv[i].y);
                }
            }

            sphereVertexArray->bind();
            sphereVertexBuffer->bind();
            sphereIndexBuffer->bind();
            sphereVertexBuffer->bufferData(data.size() * sizeof(float), data.data());
            sphereIndexBuffer->bufferData(indices.size() * sizeof(uint32_t), indices.data());
            sphereVertexArray->defineAttrib(0, 3, 8 * sizeof(float), (void*) 0);
            sphereVertexArray->defineAttrib(1, 3, 8 * sizeof(float), (void*) (3 * sizeof(float)));
            sphereVertexArray->defineAttrib(2, 2, 8 * sizeof(float), (void*) (6 * sizeof(float)));
            sphereIndexBuffer->unbind();
            sphereVertexBuffer->unbind();
            sphereVertexArray->unbind();
        }

        sphereVertexArray->bind();
        sphereIndexBuffer->bind();
        glDrawElements(GL_TRIANGLE_STRIP, sphereIndexCount, GL_UNSIGNED_INT, 0);
        sphereIndexBuffer->unbind();
        sphereVertexArray->unbind();
    }

    VertexArray* cubeVertexArray = nullptr;
    Buffer* cubeVertexBuffer = nullptr;

    void renderCube() {
        if (cubeVertexArray == nullptr) {
            std::vector <float> vertices = {
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
                -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
                 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
                -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
                 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
                 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
                 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
                 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
                -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
                 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
                 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
                 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
                -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
            };

            cubeVertexArray = new VertexArray();
            cubeVertexBuffer = new Buffer(NICKEL2_ARRAY_BUFFER);

            cubeVertexArray->bind();
            cubeVertexBuffer->bind();
            cubeVertexBuffer->bufferData(vertices.size() * sizeof(float), vertices.data());
            cubeVertexArray->defineAttrib(0, 3, 8 * sizeof(float), (void*) 0);
            cubeVertexArray->defineAttrib(1, 3, 8 * sizeof(float), (void*) (3 * sizeof(float)));
            cubeVertexArray->defineAttrib(2, 2, 8 * sizeof(float), (void*) (6 * sizeof(float)));
            cubeVertexBuffer->unbind();
            cubeVertexArray->unbind();
        }

        cubeVertexArray->bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        cubeVertexArray->unbind();
    }

    VertexArray* quadVertexArray = nullptr;
    Buffer* quadVertexBuffer = nullptr;

    void renderQuad() {
        if (quadVertexArray == nullptr) {
            std::vector <float> vertices = {
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
            };

            quadVertexArray = new VertexArray();
            quadVertexBuffer = new Buffer(NICKEL2_ARRAY_BUFFER);

            quadVertexArray->bind();
            quadVertexBuffer->bind();
            quadVertexBuffer->bufferData(vertices.size() * sizeof(float), vertices.data());
            quadVertexArray->defineAttrib(0, 3, 5 * sizeof(float), (void*) 0);
            quadVertexArray->defineAttrib(1, 2, 5 * sizeof(float), (void*) (3 * sizeof(float)));
            quadVertexBuffer->unbind();
            quadVertexArray->unbind();
        }

        quadVertexArray->bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        quadVertexArray->unbind();
    }

    void destroyRenderer() {
        if (sphereVertexArray != nullptr) {
            sphereVertexArray->destroy();
            sphereVertexBuffer->destroy();
            sphereIndexBuffer->destroy();
            delete sphereVertexArray;
            delete sphereVertexBuffer;
            delete sphereIndexBuffer;
        }

        if (cubeVertexArray != nullptr) {
            cubeVertexArray->destroy();
            cubeVertexBuffer->destroy();
            delete cubeVertexArray;
            delete cubeVertexBuffer;
        }

        if (quadVertexArray != nullptr) {
            quadVertexArray->destroy();
            quadVertexBuffer->destroy();
            delete quadVertexArray;
            delete quadVertexBuffer;
        }

        getContext()->logger->log(NICKEL2_INFO, "renderer successfully destroyed.");
    }

    uint32_t Renderer::getEnvironmentCubeMap() {
        return envCubemap;
    }

    void Renderer::removeHDRTexture() {
        if (useHDRTexture) {
            hdrTexture->destroy();
            delete hdrTexture;
            glDeleteTextures(1, &envCubemap);
            glDeleteTextures(1, &brdfLUT);
            glDeleteTextures(1, &prefilterMap);
            glDeleteFramebuffers(1, &captureFBO);
            glDeleteRenderbuffers(1, &captureRBO);
            equirectangularToCubemapShader->destroy();
            irradianceShader->destroy();
            prefilterShader->destroy();
            brdfShader->destroy();
            backgroundShader->destroy();
            delete equirectangularToCubemapShader;
            delete irradianceShader;
            delete prefilterShader;
            delete brdfShader;
            delete backgroundShader;
            useHDRTexture = false;
        }
    }

    void Renderer::loadHDRTexture(const std::string& filePath) {
        if (!useHDRTexture) {
            equirectangularToCubemapShader = new Shader(readFile("shaders/cubemap.vert"), readFile("shaders/equirectangularToCubemap.frag"));
            irradianceShader = new Shader(readFile("shaders/cubemap.vert"), readFile("shaders/irradianceConvolution.frag"));
            prefilterShader = new Shader(readFile("shaders/cubemap.vert"), readFile("shaders/prefilter.frag"));
            brdfShader = new Shader(readFile("shaders/brdf.vert"), readFile("shaders/brdf.frag"));
            backgroundShader = new Shader(readFile("shaders/background.vert"), readFile("shaders/background.frag"));
            useHDRTexture = true;
        } else {
            hdrTexture->destroy();
            delete hdrTexture;
            glDeleteTextures(1, &envCubemap);
            glDeleteTextures(1, &brdfLUT);
            glDeleteTextures(1, &prefilterMap);
            glDeleteFramebuffers(1, &captureFBO);
            glDeleteRenderbuffers(1, &captureRBO);
        }

        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

        hdrTexture = new Texture(filePath.c_str(), 0, \
            {GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_RGB16F, GL_RGB, GL_FLOAT, GL_FALSE});

        glGenTextures(1, &envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        for (uint32_t i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.01f, 10.0f);

        glm::mat4 captureViews[] = {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        equirectangularToCubemapShader->use();
        equirectangularToCubemapShader->setUniform1i("equirectangularMap", 0);
        equirectangularToCubemapShader->setUniformMatrix4fv("proj", glm::value_ptr(captureProjection));

        hdrTexture->bind();

        glViewport(0, 0, 512, 512);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

        for (uint32_t i = 0; i < 6; ++i) {
            equirectangularToCubemapShader->setUniformMatrix4fv("view", glm::value_ptr(captureViews[i]));
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderCube();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        glGenTextures(1, &irradianceMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);

        for (uint32_t i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

        irradianceShader->use();
        irradianceShader->setUniform1i("environmentMap", 0);
        irradianceShader->setUniformMatrix4fv("proj", glm::value_ptr(captureProjection));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glViewport(0, 0, 32, 32);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

        for (uint32_t i = 0; i < 6; ++i) {
            irradianceShader->setUniformMatrix4fv("view", glm::value_ptr(captureViews[i]));
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderCube();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenTextures(1, &prefilterMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);

        for (uint32_t i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        prefilterShader->use();
        prefilterShader->setUniform1i("environmentMap", 0);
        prefilterShader->setUniformMatrix4fv("proj", glm::value_ptr(captureProjection));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

        uint32_t maxMipLevels = 5;

        for (uint32_t mip = 0; mip < maxMipLevels; ++mip) {
            uint32_t mipWidth  = static_cast<uint32_t>(128 * std::pow(0.5, mip));
            uint32_t mipHeight = static_cast<uint32_t>(128 * std::pow(0.5, mip));
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);

            float roughness = (float) mip / (float) (maxMipLevels - 1);
            prefilterShader->setUniform1f("roughness", roughness);

            for (uint32_t i = 0; i < 6; ++i) {
                prefilterShader->setUniformMatrix4fv("view", glm::value_ptr(captureViews[i]));
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                renderCube();
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenTextures(1, &brdfLUT);
        glBindTexture(GL_TEXTURE_2D, brdfLUT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUT, 0);

        glViewport(0, 0, 512, 512);
        brdfShader->use();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderQuad();
        brdfShader->unuse();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        int32_t width, height;
        window->getSize(&width, &height);
        glViewport(0, 0, width, height);
    }

    void Renderer::reloadShaders() {
        if (useHDRTexture) {
            equirectangularToCubemapShader->reload(readFile("shaders/cubemap.vert"), readFile("shaders/equirectangularToCubemap.frag"));
            irradianceShader->reload(readFile("shaders/cubemap.vert"), readFile("shaders/irradianceConvolution.frag"));
            prefilterShader->reload(readFile("shaders/cubemap.vert"), readFile("shaders/prefilter.frag"));
            brdfShader->reload(readFile("shaders/brdf.vert"), readFile("shaders/brdf.frag"));
            backgroundShader->reload(readFile("shaders/background.vert"), readFile("shaders/background.frag"));
        }

        shader->reload(readFile("shaders/default.vert"), readFile("shaders/default.frag"));
        basicShader->reload(readFile("shaders/default.vert"), readFile("shaders/basic.frag"));

        if (depthCubemapShader != nullptr)
            depthCubemapShader->reload(readFile("shaders/depthCubemap.vert"), readFile("shaders/depthCubemap.frag"), readFile("shaders/depthCubemap.geom"));
        if (depthMapShader != nullptr) depthMapShader->reload(readFile("shaders/depthMap.vert"), readFile("shaders/depthMap.frag"));
        if (gBufferShader != nullptr) gBufferShader->reload(readFile("shaders/gBuffer.vert"), readFile("shaders/gBuffer.frag"));
        if (ssaoShader != nullptr) ssaoShader->reload(readFile("shaders/ssao.vert"), readFile("shaders/ssao.frag"));
        if (ssaoBlurShader != nullptr) ssaoBlurShader->reload(readFile("shaders/ssao.vert"), readFile("shaders/ssaoBlur.frag"));
    }

    void Renderer::bindTextures() {
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapPoint);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, depthMapDirectional);
        glActiveTexture(GL_TEXTURE11);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE12);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE13);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);
        glActiveTexture(GL_TEXTURE14);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);

        if (useHDRTexture) {
            glActiveTexture(GL_TEXTURE8);
            glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
            glActiveTexture(GL_TEXTURE9);
            glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
            glActiveTexture(GL_TEXTURE10);
            glBindTexture(GL_TEXTURE_2D, brdfLUT);
        }
    }

    void Renderer::renderBackground() {
        bindTextures();
        shader->use();
        
        if (useHDRTexture) {
            glDisable(GL_DEPTH_TEST);
            backgroundShader->use();
            backgroundShader->setUniform1i("environmentMap", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
            renderCube();
            backgroundShader->unuse();
            glEnable(GL_DEPTH_TEST);
        }

        shader->unuse();
    }

    void Renderer::updatePointLights(std::vector <Light>& lights) {
        std::vector<float> positions;
        std::vector<float> colors;
        std::vector<float> brightnesses;

        shader->use();
        shader->setUniform1i("pointLightCount", lights.size());

        for (uint32_t i = 0; i < lights.size(); i++) {
            shader->setUniform3fv(("pointLightPositions[" + std::to_string(i) + "]").c_str(), (float*) glm::value_ptr(lights[i].position));
            shader->setUniform3fv(("pointLightColors[" + std::to_string(i) + "]").c_str(), (float*) glm::value_ptr(lights[i].color));
            shader->setUniform1f(("pointLightBrightnesses[" + std::to_string(i) + "]").c_str(), lights[i].brightness);
        }

        shader->unuse();
    }

    void Renderer::setupShadowMaps() {
        depthCubemapShader = new Shader(readFile("shaders/depthCubemap.vert"), readFile("shaders/depthCubemap.frag"), readFile("shaders/depthCubemap.geom"));
        depthMapShader = new Shader(readFile("shaders/depthMap.vert"), readFile("shaders/depthMap.frag"));

        glDepthFunc(GL_LEQUAL);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        glGenFramebuffers(1, &depthMapPointFBO);
        glGenTextures(1, &depthMapPoint);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapPoint);

        for (uint32_t i = 0; i < 6; i++)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, depthMapSize.x, depthMapSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapPointFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMapPoint, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenFramebuffers(1, &depthMapDirectionalFBO);
        glGenTextures(1, &depthMapDirectional);
        glBindTexture(GL_TEXTURE_2D, depthMapDirectional);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthMapSize.x, depthMapSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapDirectionalFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapDirectional, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Renderer::destroyShadowMaps() {
        glDeleteFramebuffers(1, &depthMapDirectionalFBO);
        glDeleteFramebuffers(1, &depthMapPointFBO);
        glDeleteTextures(1, &depthMapDirectional);
        glDeleteTextures(1, &depthMapPoint);
        depthCubemapShader->destroy();
        depthMapShader->destroy();
        delete depthCubemapShader;
        delete depthMapShader;
        depthCubemapShader = nullptr;
        depthMapShader = nullptr;
    }

    void Renderer::updateShadowMaps(std::vector<Light>& lights) {
        if (lights.size() != 0) {
            glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), depthMapSize.x / depthMapSize.y, 0.1f, 1000.0f);
            glm::vec3 position = lights[0].position;

            std::vector <glm::mat4> shadowTransforms {
                shadowProj * glm::lookAt(position, position + glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)),
                shadowProj * glm::lookAt(position, position + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)),
                shadowProj * glm::lookAt(position, position + glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0)),
                shadowProj * glm::lookAt(position, position + glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0)),
                shadowProj * glm::lookAt(position, position + glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0)),
                shadowProj * glm::lookAt(position, position + glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0))
            };

            glViewport(0, 0, depthMapSize.x, depthMapSize.y);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapPointFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            depthCubemapShader->use();

            for (uint32_t i = 0; i < 6; i++)
                depthCubemapShader->setUniformMatrix4fv(("shadowMatrices[" + std::to_string(i) + "]").c_str(), glm::value_ptr(shadowTransforms[i]));

            depthCubemapShader->setUniform3fv("lightPosition", glm::value_ptr(position));
            depthCubemapShader->unuse();

            for (uint32_t i = 0; i < queue.size(); i++)
                queue[i]->render(depthCubemapShader, false, true);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            int32_t width, height;
            window->getSize(&width, &height);
            glViewport(0, 0, width, height);
        }
    }

    void Renderer::updateShadowMaps(Scene* scene, bool updateQueue) {
        Camera* camera = scene->getPrimaryCamera();

        if (updateQueue) {
            for (auto& entity : scene->getAllEntitiesWith<MeshComponent>())
                this->submit(scene->registry.get<MeshComponent>(entity).mesh);
        }

        int32_t width, height;
        window->getSize(&width, &height);

        glm::mat4 lightProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 100.0f);
        glm::mat4 lightView = glm::lookAt(scene->directionalLightPosition * 20.0f + camera->transform->getTranslation(), camera->transform->getTranslation(), glm::vec3(0.0f, 1.0f, 0.0f));
        directionalLightSpaceMatrix = lightProjection * lightView;

        if (scene->enableDirectionalLight) {
            depthMapShader->use();
            depthMapShader->setUniformMatrix4fv("lightSpaceMatrix", glm::value_ptr(directionalLightSpaceMatrix));
            depthMapShader->unuse();

            glViewport(0, 0, depthMapSize.x, depthMapSize.y);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapDirectionalFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            
            for (uint32_t i = 0; i < queue.size(); i++)
                queue[i]->render(depthMapShader, false, true);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, width, height);

            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, depthMapDirectional);
        }

        updateShadowMaps(scene->getLights());
        if (updateQueue) queue.clear();
    }

    void Renderer::setupGBuffer() {
        gBufferShader = new Shader(readFile("shaders/gBuffer.vert"), readFile("shaders/gBuffer.frag"));
        enableGBuffer = true;

        glGenFramebuffers(1, &gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        
        glGenTextures(1, &gPosition);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
        
        glGenTextures(1, &gNormal);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

        glGenTextures(1, &gAlbedo);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
        
        uint32_t attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);

        glGenRenderbuffers(1, &depthRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "framebuffer not complete." << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Renderer::destroyGBuffer() {
        glDeleteTextures(1, &gAlbedo);
        glDeleteTextures(1, &gPosition);
        glDeleteTextures(1, &gNormal);
        glDeleteFramebuffers(1, &gBuffer);
        glDeleteRenderbuffers(1, &depthRBO);
        gBufferShader->destroy();
        delete gBufferShader;
        gBufferShader = nullptr;
        enableGBuffer = false;
    }

    void Renderer::setupSSAO() {
        ssaoShader = new Shader(readFile("shaders/ssao.vert"), readFile("shaders/ssao.frag"));
        ssaoBlurShader = new Shader(readFile("shaders/ssao.vert"), readFile("shaders/ssaoBlur.frag"));
        enableSSAO = true;

        glGenFramebuffers(1, &ssaoFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

        glGenTextures(1, &ssaoColorBuffer);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, windowWidth, windowHeight, 0, GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "SSAO framebuffer not complete." << std::endl;

        glGenFramebuffers(1, &ssaoBlurFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);

        glGenTextures(1, &ssaoColorBufferBlur);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, windowWidth, windowHeight, 0, GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "SSAO blur framebuffer not complete." << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
        std::default_random_engine generator;

        for (uint32_t i = 0; i < 64; ++i) {
            glm::vec3 sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));
            sample = glm::normalize(sample);
            sample *= randomFloats(generator);
            float scale = float(i) / 64.0f;
            scale = glm::lerp(0.1f, 1.0f, scale * scale);
            sample *= scale;
            ssaoKernel.push_back(sample);
        }

        std::vector<glm::vec3> ssaoNoise;

        for (uint32_t i = 0; i < 16; i++) {
            glm::vec3 noise(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, 0.0f);
            ssaoNoise.push_back(noise);
        }

        glGenTextures(1, &noiseTexture);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    void Renderer::destroySSAO() {
        glDeleteTextures(1, &noiseTexture);
        glDeleteTextures(1, &ssaoColorBuffer);
        glDeleteTextures(1, &ssaoColorBufferBlur);
        glDeleteFramebuffers(1, &ssaoFBO);
        glDeleteFramebuffers(1, &ssaoBlurFBO);
        ssaoShader->destroy();
        ssaoBlurShader->destroy();
        delete ssaoShader;
        delete ssaoBlurShader;
        ssaoShader = nullptr;
        ssaoBlurShader = nullptr;
        enableSSAO = false;
    }

    Renderer::Renderer(Window* window, const std::string& hdrTexturePath, glm::ivec2 depthMapSize)
        : window(window), intro(true), consoleState(false), consoleKeyState(true), useHDRTexture(false), \
          depthMapSize(depthMapSize), imguiShouldRender(false), imguiFrameState(false) {
        shader = new Shader(readFile("shaders/default.vert"), readFile("shaders/default.frag"));
        basicShader = new Shader(readFile("shaders/default.vert"), readFile("shaders/basic.frag"));

        window->getSize(&windowWidth, &windowHeight);

        window->clear();
        basicShader->use();
        basicShader->setUniform1i("textureScale", 1);
        basicShader->setUniform1i("useAlbedoMap", 1);
        basicShader->setUniform3fv("albedoDefault", (float*) glm::value_ptr(glm::vec3(0.0f, 0.5f, 0.5f)));
        glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.3f / 3.0f * 2.0f, 0.12f / 3.0f * 2.0f, 0.0f));
        basicShader->setUniformMatrix4fv("model", (float*) glm::value_ptr(model));
        basicShader->setUniformMatrix3fv("normalMatrix", (float*) glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(model)))));
        basicShader->setUniformMatrix4fv("proj", (float*) glm::value_ptr(glm::mat4(1.0f)));
        basicShader->setUniformMatrix4fv("view", (float*) glm::value_ptr(glm::mat4(1.0f)));
        logoTexture = new Texture("res/logo.png", 0);
        logoTexture->bind();
        logoTexture->texUnit(basicShader, "albedoMap");
        backgroundTransparency = 1.0f;
        logoTransparency = 1.0f;
        renderQuad();
        basicShader->unuse();
        window->swapBuffers();

        setupShadowMaps();
        setupGBuffer();
        setupSSAO();

        if (!hdrTexturePath.empty()) {
            loadHDRTexture(hdrTexturePath);
        }
    }

    Renderer::~Renderer() {

    }

    void Renderer::imGuiNewFrame(bool setContext) {
        imguiShouldRender = true, imguiFrameState = true;
        if (setContext) ImGui::SetCurrentContext(window->getImGuiContext());
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Renderer::imGuiEndFrame() {
        imguiFrameState = false;
        ImGui::EndFrame();
    }

    void Renderer::imGuiRender() {
        imguiShouldRender = false;
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Renderer::submit(Mesh* model) {
        queue.push_back(model);
    }

    void Renderer::render(Scene* scene, bool updateShadowMaps, bool updateCamera, bool renderBackground) {
        Camera* camera = scene->getPrimaryCamera();
        std::vector <Light> lights = scene->getLights();
        updatePointLights(lights);

        int32_t width, height;
        window->getSize(&width, &height);

        if (width != windowWidth || height != windowHeight) {
            windowWidth = width, windowHeight = height;

            if (enableGBuffer)
                destroyGBuffer(), setupGBuffer();

            if (enableSSAO)
                destroySSAO(), setupSSAO();
        }

        for (auto& entity : scene->getAllEntitiesWith<MeshComponent>())
            this->submit(scene->registry.get<MeshComponent>(entity).mesh);
        
        if (updateShadowMaps)
            this->updateShadowMaps(scene, false);

        shader->use();
        shader->setUniformMatrix4fv("lightSpaceMatrix", glm::value_ptr(directionalLightSpaceMatrix));
        shader->setUniform3fv("directionalLightPosition", glm::value_ptr(scene->directionalLightPosition));
        shader->setUniform1i("enableIBL", useHDRTexture ? 1 : 0);
        shader->setUniform1i("enableSSAO", enableSSAO ? 1 : 0);
        shader->setUniform1i("enableShadows", scene->enableShadows ? 1 : 0);
        shader->setUniform1i("enableDirectionalLight", scene->enableDirectionalLight ? 1 : 0);
        shader->setUniform1i("depthMapPoint", 6);
        shader->setUniform1i("depthMapDirectional", 7);
        shader->setUniform1i("irradianceMap", 8);
        shader->setUniform1i("prefilterMap", 9);
        shader->setUniform1i("brdfLUT", 10);
        shader->setUniform1i("gPosition", 11);
        shader->setUniform1i("gNormal", 12);
        shader->setUniform1i("gAlbedo", 13);
        shader->setUniform1i("ssaoBlur", 14);
        shader->unuse();

        if (updateCamera) {
            camera->updateMatrices(shader);
            camera->updateMatrices(basicShader);
        }

        if (useHDRTexture && renderBackground) {
            if (updateCamera)
                camera->updateMatrices(backgroundShader);

            this->renderBackground();
        }

        if (enableGBuffer) {
            glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            camera->updateMatrices(gBufferShader);

            for (uint32_t i = 0; i < queue.size(); i++)
                queue[i]->render(gBufferShader, false, true);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(window->backgroundColor.r / 255.0f, window->backgroundColor.g / 255.0f, window->backgroundColor.b / 255.0f, window->backgroundColor.a / 255.0f);
        }

        if (enableSSAO) {
            if (!enableGBuffer) {
                Nickel2::getContext()->logger->log(NICKEL2_ERROR, "SSAO requires G-buffer to be enabled.");
                return;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
            glClear(GL_COLOR_BUFFER_BIT);
            camera->updateMatrices(ssaoShader);
            ssaoShader->use();
            ssaoShader->setUniform1i("gPosition", 0);
            ssaoShader->setUniform1i("gNormal", 1);
            ssaoShader->setUniform1i("texNoise", 2);
            ssaoShader->setUniform2fv("windowSize", (float*) glm::value_ptr(glm::vec2(width, height)));

            for (unsigned int i = 0; i < 64; ++i)
                ssaoShader->setUniform3fv("samples[" + std::to_string(i) + "]", glm::value_ptr(ssaoKernel[i]));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gPosition);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gNormal);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, noiseTexture);
            renderQuad();
            ssaoShader->unuse();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
            glClear(GL_COLOR_BUFFER_BIT);
            ssaoBlurShader->use();
            ssaoBlurShader->setUniform1i("ssaoInput", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
            renderQuad();
            ssaoBlurShader->unuse();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        bindTextures();

        for (uint32_t i = 0; i < queue.size(); i++)
            queue[i]->render(shader);

        queue.clear();

        if (!imguiFrameState || !imguiShouldRender)
            imGuiNewFrame();

        ImGui::BeginChild("Render");
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((void*) (intptr_t) depthMapDirectional, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::Image((void*) (intptr_t) ssaoColorBuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::Image((void*) (intptr_t) gPosition, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::Image((void*) (intptr_t) gNormal, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::Image((void*) (intptr_t) gAlbedo, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();

        ImGui::Checkbox("Enable Directional Light", &scene->enableDirectionalLight);
        ImGui::Checkbox("Enable Shadows", &scene->enableShadows);
        enableSSAO = scene->enableShadows;
        ImGui::Checkbox("Enable SSAO", &enableSSAO);
        scene->enableShadows = enableSSAO;

        if (window->input->getKey(NICKEL2_KEY_GRAVE_ACCENT)) {
            if (consoleKeyState)
                consoleKeyState = false, consoleState = !consoleState;
        } else {
            consoleKeyState = true;
        }

        if (consoleState) {
            ImGui::Begin("Console", &consoleState, ImGuiWindowFlags_NoResize);
            ImGui::SetWindowSize(ImVec2(width / 2, height / 2));

            for (std::string i : context->logger->getLogs())
                ImGui::Text(i.c_str());

            ImGui::End();
        }

        if (imguiFrameState) imGuiEndFrame();
        if (imguiShouldRender) imGuiRender();

        if (intro) {
            glEnable(GL_BLEND);
            glDisable(GL_DEPTH_TEST);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            if (backgroundTransparency != 0.0f) {
                basicShader->use();
                basicShader->setUniform1i("useAlbedoMap", 0);
                basicShader->setUniform1i("isTransparent", 1);
                basicShader->setUniform1f("transparency", backgroundTransparency);
                basicShader->setUniform3fv("albedoDefault", (float*) glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
                glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 0.0f));
                basicShader->setUniformMatrix4fv("model", (float*) glm::value_ptr(model));;
                basicShader->setUniformMatrix3fv("normalMatrix", (float*) glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(model)))));
                basicShader->setUniformMatrix4fv("proj", (float*) glm::value_ptr(glm::mat4(1.0f)));
                basicShader->setUniformMatrix4fv("view", (float*) glm::value_ptr(glm::mat4(1.0f)));
                renderQuad();
                basicShader->unuse();

                if (backgroundTransparency > 0.0f)
                    backgroundTransparency -= 0.6f * window->deltaTime;
                
                if (backgroundTransparency < 0.0f)
                    backgroundTransparency = 0.0f;
            }

            if (logoTransparency != 0.0f) {
                basicShader->use();
                basicShader->setUniform1i("textureScale", 1);
                basicShader->setUniform1i("useAlbedoMap", 1);
                basicShader->setUniform1i("isTransparent", 1);
                basicShader->setUniform1f("transparency", logoTransparency);
                glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.3f / 3.0f * 2.0f, 0.12f / 3.0f * 2.0f, 0.0f));
                basicShader->setUniformMatrix4fv("model", (float*) glm::value_ptr(model));
                basicShader->setUniformMatrix3fv("normalMatrix", (float*) glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(model)))));
                basicShader->setUniformMatrix4fv("proj", (float*) glm::value_ptr(glm::mat4(1.0f)));
                basicShader->setUniformMatrix4fv("view", (float*) glm::value_ptr(glm::mat4(1.0f)));
                logoTexture->bind();
                logoTexture->texUnit(basicShader, "albedoMap");
                renderQuad();
                basicShader->unuse();

                if (logoTransparency > 0.0f)
                    logoTransparency -= 0.5f * window->deltaTime;
                
                if (logoTransparency < 0.0f)
                    logoTransparency = 0.0f;
            } else {
                intro = false;
            }

            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
        }
    }

    void Renderer::destroy() {
        logoTexture->destroy();
        shader->destroy();
        basicShader->destroy();
        delete logoTexture;
        delete shader;
        delete basicShader;

        destroyShadowMaps();
        destroyGBuffer();
        destroySSAO();

        if (useHDRTexture) {
            hdrTexture->destroy();
            delete hdrTexture;
            glDeleteTextures(1, &envCubemap);
            glDeleteTextures(1, &brdfLUT);
            glDeleteTextures(1, &prefilterMap);
            glDeleteFramebuffers(1, &captureFBO);
            glDeleteRenderbuffers(1, &captureRBO);
            equirectangularToCubemapShader->destroy();
            irradianceShader->destroy();
            prefilterShader->destroy();
            brdfShader->destroy();
            backgroundShader->destroy();
            delete equirectangularToCubemapShader;
            delete irradianceShader;
            delete prefilterShader;
            delete brdfShader;
            delete backgroundShader;
        }
    }
}