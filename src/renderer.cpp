#include <nickel2/renderer.hpp>

namespace nickel2 {
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
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
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

    void Renderer::removeHDRTexture() {
        if (useHDRTexture) {
            hdrTexture->destroy();
            delete hdrTexture;
            glDeleteTextures(1, &envCubemap);
            glDeleteTextures(1, &brdfLUTTexture);
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
            glDeleteTextures(1, &brdfLUTTexture);
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
            uint32_t mipWidth  = static_cast <uint32_t>(128 * std::pow(0.5, mip));
            uint32_t mipHeight = static_cast <uint32_t>(128 * std::pow(0.5, mip));
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

        glGenTextures(1, &brdfLUTTexture);
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

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
        depthShader->reload(readFile("shaders/depth.vert"), readFile("shaders/depth.frag"), readFile("shaders/depth.geom"));
    }

    void Renderer::bindTextures() {
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

        if (useHDRTexture) {
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
            glActiveTexture(GL_TEXTURE8);
            glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
            glActiveTexture(GL_TEXTURE9);
            glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
        }
    }

    void Renderer::renderBackground() {
        bindTextures();
        shader->use();
        
        if (useHDRTexture) {
            glDisable(GL_DEPTH_TEST);
            backgroundShader->use();
            backgroundShader->setUniform1i("environmentMap", 10);
            glActiveTexture(GL_TEXTURE10);
            glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
            renderCube();
            backgroundShader->unuse();
            glEnable(GL_DEPTH_TEST);
        }

        shader->unuse();
    }

    void Renderer::updateLights(std::vector <Light>& lights) {
        std::vector <float> positions;
        std::vector <float> colors;
        std::vector <float> brightnesses;

        shader->use();
        shader->setUniform1i("lightCount", lights.size());

        for (uint32_t i = 0; i < lights.size(); i++) {
            shader->setUniform3fv(("lightPositions[" + std::to_string(i) + "]").c_str(), (float*) glm::value_ptr(lights[i].position));
            shader->setUniform3fv(("lightColors[" + std::to_string(i) + "]").c_str(), (float*) glm::value_ptr(lights[i].color));
            shader->setUniform1f(("lightBrightnesses[" + std::to_string(i) + "]").c_str(), lights[i].brightness);
        }

        shader->unuse();
    }

    void Renderer::updateShadowMap(std::vector <Light>& lights) {
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
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            depthShader->use();

            for (uint32_t i = 0; i < 6; i++)
                depthShader->setUniformMatrix4fv(("shadowMatrices[" + std::to_string(i) + "]").c_str(), glm::value_ptr(shadowTransforms[i]));

            depthShader->setUniform3fv("lightPosition", glm::value_ptr(position));

            for (uint32_t i = 0; i < queue.size(); i++) {
                queue[i]->render(depthShader, false);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            int32_t width, height;
            window->getSize(&width, &height);
            glViewport(0, 0, width, height);
        }
    }

    void Renderer::updateShadowMap(Scene* scene) {
        for (Model* object : scene->getObjects())
            this->submit(object);

        updateShadowMap(scene->getLights());
        queue.clear();
    }

    Renderer::Renderer(Window* window, const std::string& hdrTexturePath, glm::ivec2 depthMapSize) : window(window), depthMapSize(depthMapSize) {
        shader = new Shader(readFile("shaders/default.vert"), readFile("shaders/default.frag"));
        basicShader = new Shader(readFile("shaders/default.vert"), readFile("shaders/basic.frag"));
        depthShader = new Shader(readFile("shaders/depth.vert"), readFile("shaders/depth.frag"), readFile("shaders/depth.geom"));
        useHDRTexture = false, intro = true;

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

        glDepthFunc(GL_LEQUAL);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        glGenFramebuffers(1, &depthMapFBO);
        glGenTextures(1, &depthCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

        for (uint32_t i = 0; i < 6; i++)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, depthMapSize.x, depthMapSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (!hdrTexturePath.empty()) {
            loadHDRTexture(hdrTexturePath);
        }
    }

    Renderer::~Renderer() {

    }

    void Renderer::submit(Model* model) {
        queue.push_back(model);
    }

    void Renderer::render(Camera* camera, Scene* scene, bool updateShadowMap, bool updateCamera) {
        std::vector <Light> lights = scene->getLights();
        updateLights(lights);
        bindTextures();

        for (Model* object : scene->getObjects())
            this->submit(object);

        shader->use();
        shader->setUniform1i("enableIBL", useHDRTexture ? 1 : 0);
        shader->setUniform1i("enableShadows", 1);
        shader->setUniform1i("depthMap", 6);
        shader->setUniform1i("irradianceMap", 7);
        shader->setUniform1i("prefilterMap", 8);
        shader->setUniform1i("brdfLUT", 9);
        shader->unuse();
        
        if (updateShadowMap)
            this->updateShadowMap(lights);

        if (updateCamera) {
            camera->updateMatrices(shader);
            camera->updateMatrices(basicShader);
        }

        if (useHDRTexture) {
            if (updateCamera)
                camera->updateMatrices(backgroundShader);

            renderBackground();
        }

        for (uint32_t i = 0; i < queue.size(); i++)
            queue[i]->render(shader);

        queue.clear();

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
        delete logoTexture;
        glDeleteFramebuffers(1, &depthMapFBO);
        shader->destroy();
        basicShader->destroy();
        depthShader->destroy();
        delete shader;
        delete basicShader;
        delete depthShader;

        if (useHDRTexture) {
            hdrTexture->destroy();
            delete hdrTexture;
            glDeleteTextures(1, &envCubemap);
            glDeleteTextures(1, &brdfLUTTexture);
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