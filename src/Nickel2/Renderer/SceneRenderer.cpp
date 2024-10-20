#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/Renderer.hpp>
#include <Nickel2/Renderer/SceneRenderer.hpp>
#include <Nickel2/Renderer/Framebuffer.hpp>
#include <Nickel2/Renderer/Texture.hpp>
#include <Nickel2/Renderer/Camera.hpp>
#include <Nickel2/Scene/Entity.hpp>

namespace Nickel2 {
    std::shared_ptr<VertexArray> sphereVertexArray, cubeVertexArray, quadVertexArray;
    std::shared_ptr<VertexBuffer> sphereVertexBuffer, cubeVertexBuffer, quadVertexBuffer;
    std::shared_ptr<IndexBuffer> sphereIndexBuffer;

    void RenderSphere() {
        if (sphereVertexArray == nullptr) {
            std::vector<glm::vec3> positions;
            std::vector<glm::vec2> uv;
            std::vector<glm::vec3> normals;
            std::vector<uint32_t> indices;

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
                } oddRow = !oddRow;
            }

            std::vector<float> data;

            for (uint32_t i = 0; i < positions.size(); ++i) {
                data.push_back(positions[i].x);
                data.push_back(positions[i].y);
                data.push_back(positions[i].z);
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }

            sphereVertexArray = VertexArray::Create();
            sphereVertexBuffer = VertexBuffer::Create(data.data(), data.size() * sizeof(float));
            sphereIndexBuffer = IndexBuffer::Create(indices.data(), indices.size());

            sphereVertexBuffer->SetLayout({
                { Nickel2::DataType::Float3, "position" },
                { Nickel2::DataType::Float2, "texCoord" },
                { Nickel2::DataType::Float3, "color" }
            });

            sphereVertexArray->AddVertexBuffer(sphereVertexBuffer);
            sphereVertexArray->AddIndexBuffer(sphereIndexBuffer);
        }

        sphereVertexArray->Bind();
        glDrawElements(GL_TRIANGLE_STRIP, sphereIndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
    }

    void RenderCube() {
        if (cubeVertexArray == nullptr) {
            std::vector<float> vertices = {
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

            cubeVertexArray = VertexArray::Create();
            cubeVertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));

            cubeVertexBuffer->SetLayout({
                { Nickel2::DataType::Float3, "position" },
                { Nickel2::DataType::Float2, "texCoord" },
                { Nickel2::DataType::Float3, "color" }
            });

            cubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
        }

        cubeVertexArray->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void RenderQuad() {
        if (quadVertexArray == nullptr) {
            std::vector<float> vertices = {
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
            };

            quadVertexArray = VertexArray::Create();
            quadVertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));

            quadVertexBuffer->SetLayout({
                { Nickel2::DataType::Float3, "position" },
                { Nickel2::DataType::Float2, "texCoord" }
            });

            quadVertexArray->AddVertexBuffer(quadVertexBuffer);
        }

        quadVertexArray->Bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void SceneRenderer::SetupShadowMaps() {
        depthCubeMapShader = shaderLibrary.Load("depthCubeMap", {
            { ShaderStage::Vertex, "shaders/depthCubeMap.vert" },
            { ShaderStage::Fragment, "shaders/depthCubeMap.frag" },
            { ShaderStage::Geometry, "shaders/depthCubeMap.geom" }
        });

        depthMapShader = shaderLibrary.Load("depthMap", {
            { ShaderStage::Vertex, "shaders/depthMap.vert" },
            { ShaderStage::Fragment, "shaders/depthMap.frag" }
        });

        glDepthFunc(GL_LEQUAL);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        glGenFramebuffers(1, &depthMapPointFrameBuffer);
        glGenTextures(1, &depthMapPoint);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapPoint);

        for (uint32_t i = 0; i < 6; i++)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, depthMapSize.x, depthMapSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapPointFrameBuffer);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMapPoint, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenFramebuffers(1, &depthMapDirectionalFrameBuffer);
        glGenTextures(1, &depthMapDirectional);
        glBindTexture(GL_TEXTURE_2D, depthMapDirectional);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthMapSize.x, depthMapSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapDirectionalFrameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapDirectional, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void SceneRenderer::DestroyShadowMaps() {
        glDeleteFramebuffers(1, &depthMapDirectionalFrameBuffer);
        glDeleteFramebuffers(1, &depthMapPointFrameBuffer);
        glDeleteTextures(1, &depthMapDirectional);
        glDeleteTextures(1, &depthMapPoint);
        depthCubeMapShader = nullptr, shaderLibrary.Free("depthCubeMap");
        depthMapShader = nullptr, shaderLibrary.Free("depthMap");
    }

    void SceneRenderer::SetupGBuffer() {
        gBufferShader = shaderLibrary.Load("gBuffer", {
            { ShaderStage::Vertex, "shaders/gBuffer.vert" },
            { ShaderStage::Fragment, "shaders/gBuffer.frag" }
        });

        glGenFramebuffers(1, &gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        
        glGenTextures(1, &gPosition);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowSize.x, windowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
        
        glGenTextures(1, &gNormal);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowSize.x, windowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

        glGenTextures(1, &gAlbedo);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowSize.x, windowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
        
        uint32_t attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);

        glGenRenderbuffers(1, &depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowSize.x, windowSize.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Nickel2::Logger::Log(Nickel2::Logger::Level::Info, "SceneRenderer", "Failed to create G-buffer.");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        enableGBuffer = true;
    }

    void SceneRenderer::DestroyGBuffer() {
        glDeleteTextures(1, &gAlbedo);
        glDeleteTextures(1, &gPosition);
        glDeleteTextures(1, &gNormal);
        glDeleteFramebuffers(1, &gBuffer);
        glDeleteRenderbuffers(1, &depthRenderBuffer);
        gBufferShader = nullptr, shaderLibrary.Free("gBuffer");
        enableGBuffer = false;
    }

    void SceneRenderer::SetupSSAO() {
        ssaoShader = shaderLibrary.Load("ssao", {
            { ShaderStage::Vertex, "shaders/ssao.vert" },
            { ShaderStage::Fragment, "shaders/ssao.frag" }
        });

        ssaoBlurShader = shaderLibrary.Load("ssaoBlur", {
            { ShaderStage::Vertex, "shaders/ssao.vert" },
            { ShaderStage::Fragment, "shaders/ssaoBlur.frag" }
        });

        glGenFramebuffers(1, &ssaoFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFrameBuffer);

        glGenTextures(1, &ssaoColorBuffer);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, windowSize.x, windowSize.y, 0, GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Nickel2::Logger::Log(Nickel2::Logger::Level::Info, "SceneRenderer", "Failed to create SSAO framebuffer.");

        glGenFramebuffers(1, &ssaoBlurFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFrameBuffer);

        glGenTextures(1, &ssaoColorBufferBlur);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, windowSize.x, windowSize.y, 0, GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Nickel2::Logger::Log(Nickel2::Logger::Level::Info, "SceneRenderer", "Failed to create SSAO blur framebuffer.");

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
        enableSSAO = true;
    }

    void SceneRenderer::DestroySSAO() {
        glDeleteTextures(1, &noiseTexture);
        glDeleteTextures(1, &ssaoColorBuffer);
        glDeleteTextures(1, &ssaoColorBufferBlur);
        glDeleteFramebuffers(1, &ssaoFrameBuffer);
        glDeleteFramebuffers(1, &ssaoBlurFrameBuffer);
        ssaoShader = nullptr, shaderLibrary.Free("ssao");
        ssaoBlurShader = nullptr, shaderLibrary.Free("ssaoBlur");
        enableSSAO = false;
    }

    void SceneRenderer::SetupPostProcessing() {
        glm::vec2 size = window->GetSize();

        postProcessingShader = shaderLibrary.Load("postProcessing", {
            { ShaderStage::Vertex, "shaders/default.vert" },
            { ShaderStage::Fragment, "shaders/postProcessing.frag" }
        });

        bloomBlurShader = shaderLibrary.Load("bloomBlur", {
            { ShaderStage::Vertex, "shaders/bloomBlur.vert" },
            { ShaderStage::Fragment, "shaders/bloomBlur.frag" }
        });

        glGenFramebuffers(1, &postProcessingFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFrameBuffer);
        glGenTextures(2, postProcessingColorBuffers);

        for (uint32_t i = 0; i < 2; i++) {
            glBindTexture(GL_TEXTURE_2D, postProcessingColorBuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, postProcessingColorBuffers[i], 0);
        }

        glGenRenderbuffers(1, &postProcessingRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, postProcessingRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, postProcessingRenderBuffer);
        uint32_t attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, attachments);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Nickel2::Logger::Log(Nickel2::Logger::Level::Info, "SceneRenderer", "Failed to create post-processing framebuffer.");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glGenFramebuffers(2, pingPongFrameBuffers);
        glGenTextures(2, pingPongColorBuffers);

        for (uint32_t i = 0; i < 2; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, pingPongFrameBuffers[i]);
            glBindTexture(GL_TEXTURE_2D, pingPongColorBuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingPongColorBuffers[i], 0);
            
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                Nickel2::Logger::Log(Nickel2::Logger::Level::Info, "SceneRenderer", "Failed to create ping-pong framebuffer.");
        }
    }

    void SceneRenderer::DestroyPostProcessing() {
        postProcessingShader = nullptr, shaderLibrary.Free("postProcessing");
        glDeleteTextures(2, pingPongColorBuffers);
        glDeleteFramebuffers(2, pingPongFrameBuffers);
        glDeleteTextures(2, postProcessingColorBuffers);
        glDeleteRenderbuffers(1, &postProcessingRenderBuffer);
        glDeleteFramebuffers(1, &postProcessingFrameBuffer);
    }

    void SceneRenderer::BindPostProcessingFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFrameBuffer);
    }

    void SceneRenderer::UnbindPostProcessingFramebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void SceneRenderer::FreeSkybox() {
        if (!enableSkybox) { return; }
        else { enableSkybox = false; }

        hdrTexture = nullptr;
        glDeleteTextures(1, &envCubeMap);
        glDeleteTextures(1, &brdfLUT);
        glDeleteTextures(1, &prefilterMap);
        glDeleteFramebuffers(1, &captureFrameBuffer);
        glDeleteRenderbuffers(1, &captureRenderBuffer);

        equirectangularToCubeMapShader = nullptr, shaderLibrary.Free("equirectangularToCubeMap");
        irradianceShader = nullptr, shaderLibrary.Free("irradiance");
        prefilterShader = nullptr, shaderLibrary.Free("prefilter");
        brdfShader = nullptr, shaderLibrary.Free("brdf");
        backgroundShader = nullptr, shaderLibrary.Free("background");
    }

    void SceneRenderer::LoadSkybox(const std::string& filePath) {
        if (enableSkybox) {
            FreeSkybox();
        } enableSkybox = true;

        equirectangularToCubeMapShader = shaderLibrary.Load("equirectangularToCubeMap", {
            { ShaderStage::Vertex, "shaders/cubeMap.vert" },
            { ShaderStage::Fragment, "shaders/equirectangularToCubeMap.frag" }
        });

        irradianceShader = shaderLibrary.Load("irradiance", {
            { ShaderStage::Vertex, "shaders/cubeMap.vert" },
            { ShaderStage::Fragment, "shaders/irradianceConvolution.frag" }
        });

        prefilterShader = shaderLibrary.Load("prefilter", {
            { ShaderStage::Vertex, "shaders/cubeMap.vert" },
            { ShaderStage::Fragment, "shaders/prefilter.frag" }
        });

        brdfShader = shaderLibrary.Load("brdf", {
            { ShaderStage::Vertex, "shaders/brdf.vert" },
            { ShaderStage::Fragment, "shaders/brdf.frag" }
        });

        backgroundShader = shaderLibrary.Load("background", {
            { ShaderStage::Vertex, "shaders/background.vert" },
            { ShaderStage::Fragment, "shaders/background.frag" }
        });

        glGenFramebuffers(1, &captureFrameBuffer);
        glGenRenderbuffers(1, &captureRenderBuffer);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFrameBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRenderBuffer);

        glGenTextures(1, &envCubeMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);

        for (uint32_t i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);

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

        equirectangularToCubeMapShader->Bind();
        equirectangularToCubeMapShader->SetInt("equirectangularMap", 0);
        equirectangularToCubeMapShader->SetMat4("proj", captureProjection);

        hdrTexture = Texture2D::Create(filePath.c_str());
        hdrTexture->Bind();

        glViewport(0, 0, 512, 512);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFrameBuffer);

        for (uint32_t i = 0; i < 6; ++i) {
            equirectangularToCubeMapShader->SetMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubeMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            RenderCube();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        glGenTextures(1, &irradianceMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);

        for (uint32_t i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFrameBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

        irradianceShader->Bind();
        irradianceShader->SetInt("environmentMap", 0);
        irradianceShader->SetMat4("proj", captureProjection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);

        glViewport(0, 0, 32, 32);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFrameBuffer);

        for (uint32_t i = 0; i < 6; ++i) {
            irradianceShader->SetMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            RenderCube();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenTextures(1, &prefilterMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);

        for (uint32_t i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        prefilterShader->Bind();
        prefilterShader->SetInt("environmentMap", 0);
        prefilterShader->SetMat4("proj", captureProjection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFrameBuffer);

        uint32_t maxMipLevels = 5;

        for (uint32_t mip = 0; mip < maxMipLevels; ++mip) {
            uint32_t mipWidth  = static_cast<uint32_t>(128 * std::pow(0.5, mip));
            uint32_t mipHeight = static_cast<uint32_t>(128 * std::pow(0.5, mip));
            glBindRenderbuffer(GL_RENDERBUFFER, captureRenderBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);

            float roughness = (float) mip / (float) (maxMipLevels - 1);
            prefilterShader->SetFloat("roughness", roughness);

            for (uint32_t i = 0; i < 6; ++i) {
                prefilterShader->SetMat4("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                RenderCube();
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

        glBindFramebuffer(GL_FRAMEBUFFER, captureFrameBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUT, 0);

        glViewport(0, 0, 512, 512);
        brdfShader->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderQuad();
        brdfShader->Unbind();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glm::vec2 size = window->GetSize();
        glViewport(0, 0, size.x, size.y);
    }

    void SceneRenderer::UpdatePointLights(std::vector<Light>& lights) {
        std::vector<glm::vec4> colors;
        std::vector<glm::vec3> positions;
        std::vector<float> brightnesses;

        for (uint32_t i = 0; i < lights.size(); i++)
            positions.push_back(lights[i].position), colors.push_back(glm::vec4(lights[i].color, lights[i].brightness));

        defaultShader->Bind();
        defaultShader->SetInt("pointLightCount", lights.size());
        defaultShader->SetFloatArray3("pointLightPositions", positions.data(), lights.size());
        defaultShader->SetFloatArray4("pointLightColors", colors.data(), lights.size());
        defaultShader->Unbind();
    }

    void SceneRenderer::UpdateShadowMaps(std::vector<Light>& lights) {
        if (lights.size() == 0) return;
        glm::vec3 position = lights[0].position;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), depthMapSize.x / depthMapSize.y, 0.1f, 1000.0f);

        std::vector<glm::mat4> shadowTransforms {
            shadowProj * glm::lookAt(position, position + glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)),
            shadowProj * glm::lookAt(position, position + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0)),
            shadowProj * glm::lookAt(position, position + glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0)),
            shadowProj * glm::lookAt(position, position + glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0)),
            shadowProj * glm::lookAt(position, position + glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0)),
            shadowProj * glm::lookAt(position, position + glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0))
        };

        glViewport(0, 0, depthMapSize.x, depthMapSize.y);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapPointFrameBuffer);
        glClear(GL_DEPTH_BUFFER_BIT);

        depthCubeMapShader->Bind();

        for (uint32_t i = 0; i < shadowTransforms.size(); i++)
            depthCubeMapShader->SetMat4(("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);

        depthCubeMapShader->SetFloat3("lightPosition", position);
        depthCubeMapShader->Unbind();

        for (uint32_t i = 0; i < queue.size(); i++)
            queue[i]->Render(depthCubeMapShader, false, true);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glm::vec2 size = window->GetSize();
        glViewport(0, 0, size.x, size.y);
    }

    void SceneRenderer::UpdateShadowMaps(Scene* scene, bool updateQueue) {
        Entity* camera = scene->GetPrimaryCameraEntity();

        if (updateQueue) {
            for (Entity* entity : scene->GetAllEntitiesWith<MeshComponent>())
                this->Submit(entity->GetComponent<MeshComponent>().mesh);
        }

        defaultShader->Bind();
        defaultShader->SetInt("enableDirectionalLight", scene->GetEntityCountWith<DirectionalLightComponent>() > 0 ? 1 : 0);
        defaultShader->Unbind();

        uint32_t count = 0;

        for (Entity* entity : scene->GetAllEntitiesWith<DirectionalLightComponent>()) {
            NK_CORE_ASSERT(++count == 1, "Maximum directional light count exceeded.");

            DirectionalLightComponent& directionalLight = entity->GetComponent<DirectionalLightComponent>();
            glm::vec3 directionalLightTranslation = entity->GetComponent<TransformComponent>().GetTranslation();
            glm::vec3 cameraTranslation = camera->GetComponent<TransformComponent>().GetTranslation();

            glm::mat4 lightProjection = \
                glm::ortho(-directionalLight.size, directionalLight.size, -directionalLight.size, directionalLight.size, directionalLight.nearPlane, directionalLight.farPlane);

            glm::vec3 target = directionalLight.followCamera ? cameraTranslation : directionalLightTranslation;
            glm::mat4 lightView = glm::lookAt(directionalLight.direction * directionalLight.distance + target, target, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 directionalLightSpaceMatrix = lightProjection * lightView;

            defaultShader->Bind();
            defaultShader->SetMat4("lightSpaceMatrix", directionalLightSpaceMatrix);
            defaultShader->SetFloat3("directionalLightPosition", directionalLight.direction);
            defaultShader->SetInt("enableShadowsDirectional", directionalLight.castShadows ? 1 : 0);
            defaultShader->Unbind();

            depthMapShader->Bind();
            depthMapShader->SetMat4("lightSpaceMatrix", directionalLightSpaceMatrix);
            depthMapShader->Unbind();

            glViewport(0, 0, depthMapSize.x, depthMapSize.y);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapDirectionalFrameBuffer);
            glClear(GL_DEPTH_BUFFER_BIT);
            
            for (uint32_t i = 0; i < queue.size(); i++)
                queue[i]->Render(depthMapShader, false, true);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glm::vec2 size = window->GetSize();
            glViewport(0, 0, size.x, size.y);

            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, depthMapDirectional);
        }

        UpdateShadowMaps(scene->GetLights());
        if (updateQueue) queue.clear();
    }

    SceneRenderer::SceneRenderer(Window* window, const std::string& skyboxPath) : window(window) {
        defaultShader = shaderLibrary.Load("default", {
            { ShaderStage::Vertex, "shaders/default.vert" },
            { ShaderStage::Fragment, "shaders/default.frag" }
        });

        basicShader = shaderLibrary.Load("basic", {
            { ShaderStage::Vertex, "shaders/default.vert" },
            { ShaderStage::Fragment, "shaders/basic.frag" }
        });

        windowSize = window->GetSize();

        RenderCommand::Clear();
        basicShader->Bind();
        basicShader->SetInt("textureScale", 1);
        basicShader->SetInt("useAlbedoMap", 1);
        basicShader->SetInt("isTransparent", 0);
        basicShader->SetFloat3("albedoDefault", glm::vec3(0.0f, 0.5f, 0.5f));
        glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.3f / 3.0f * 2.0f, 0.12f / 3.0f * 2.0f, 0.0f));
        basicShader->SetMat4("model", model);
        basicShader->SetMat3("inverseModel", glm::transpose(glm::inverse(glm::mat3(model))));
        basicShader->SetMat4("view", glm::mat4(1.0f));
        basicShader->SetMat4("proj", glm::mat4(1.0f));
        basicShader->SetInt("albedoMap", 0);
        logoTexture = Texture2D::Create("res/logo.png");
        logoTexture->Bind();
        RenderQuad();
        basicShader->Unbind();
        window->SwapBuffers();

        SetupPostProcessing();
        SetupShadowMaps();
        SetupGBuffer();
        SetupSSAO();

        if (!skyboxPath.empty())
            LoadSkybox(skyboxPath);
    }

    void SceneRenderer::Submit(Mesh* model) {
        queue.push_back(model);
    }

    void SceneRenderer::Render(Scene* scene, float deltaTime, float shadownUpdateInterval, bool updateCamera, bool renderBackground) {
        Camera* camera = scene->GetPrimaryCamera();
        UpdatePointLights(scene->GetLights());

        defaultShader->Bind();
        defaultShader->SetInt("enableIBL", enableSkybox ? 1 : 0);
        defaultShader->SetInt("enableSSAO", enableSSAO ? 1 : 0);
        defaultShader->SetInt("enableShadowsPoint", 1);
        defaultShader->SetInt("depthMapPoint", 6);
        defaultShader->SetInt("depthMapDirectional", 7);
        defaultShader->SetInt("irradianceMap", 8);
        defaultShader->SetInt("prefilterMap", 9);
        defaultShader->SetInt("brdfLUT", 10);
        defaultShader->SetInt("gPosition", 11);
        defaultShader->SetInt("gNormal", 12);
        defaultShader->SetInt("gAlbedo", 13);
        defaultShader->SetInt("ssaoBlur", 14);
        defaultShader->Unbind();

        glm::vec2 size = window->GetSize();

        if (size.x != windowSize.x || size.y != windowSize.y) {
            windowSize.x = size.x, windowSize.y = size.y;
            DestroyPostProcessing(), SetupPostProcessing();

            if (enableGBuffer)
                DestroyGBuffer(), SetupGBuffer();

            if (enableSSAO)
                DestroySSAO(), SetupSSAO();
        }

        for (Entity* entity : scene->GetAllEntitiesWith<MeshComponent>())
            this->Submit(entity->GetComponent<MeshComponent>().mesh);

        static float shadowUpdateCooldown = 0.0f;

        if (shadowUpdateCooldown <= 0.0f) {
            this->UpdateShadowMaps(scene, false);
            shadowUpdateCooldown = shadownUpdateInterval;
        } shadowUpdateCooldown -= deltaTime;

        if (updateCamera) {
            camera->UpdateMatrices(defaultShader);
            camera->UpdateMatrices(basicShader);

            if (enableSkybox && renderBackground)
                camera->UpdateMatrices(backgroundShader);
        }

        if (enableGBuffer) {
            glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            camera->UpdateMatrices(gBufferShader);

            for (uint32_t i = 0; i < queue.size(); i++)
                queue[i]->Render(gBufferShader, false, true);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            RenderCommand::SetClearColor(clearColor);
        }

        if (enableSSAO) {
            if (!enableGBuffer) {
                Nickel2::Logger::Log(Nickel2::Logger::Level::Error, "SceneRenderer", "SSAO requires G-buffer to be enabled.");
                return;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, ssaoFrameBuffer);
            glClear(GL_COLOR_BUFFER_BIT);

            camera->UpdateMatrices(ssaoShader);
            
            ssaoShader->Bind();
            ssaoShader->SetInt("gPosition", 0);
            ssaoShader->SetInt("gNormal", 1);
            ssaoShader->SetInt("texNoise", 2);
            ssaoShader->SetFloat2("windowSize", size);
            ssaoShader->SetFloatArray3("samples", ssaoKernel.data(), ssaoKernel.size());

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gPosition);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gNormal);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, noiseTexture);
            RenderQuad();
            ssaoShader->Unbind();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFrameBuffer);
            glClear(GL_COLOR_BUFFER_BIT);
            ssaoBlurShader->Bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
            RenderQuad();
            ssaoBlurShader->Unbind();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        BindPostProcessingFramebuffer();

        if (enableSkybox && renderBackground) {
            RenderCommand::Clear();
            this->RenderBackground();
        }

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

        if (enableSkybox) {
            glActiveTexture(GL_TEXTURE8);
            glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
            glActiveTexture(GL_TEXTURE9);
            glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
            glActiveTexture(GL_TEXTURE10);
            glBindTexture(GL_TEXTURE_2D, brdfLUT);
        }

        for (uint32_t i = 0; i < queue.size(); i++)
            queue[i]->Render(defaultShader);

        queue.clear();
        UnbindPostProcessingFramebuffer();

        bool horizontal = true;
        bloomBlurShader->Bind();
        bloomBlurShader->SetInt("image", 0);

        for (uint32_t i = 0; i < 16; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, pingPongFrameBuffers[horizontal]);
            bloomBlurShader->SetInt("horizontal", horizontal);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, i == 0 ? postProcessingColorBuffers[1] : pingPongColorBuffers[!horizontal]);
            RenderQuad();
            horizontal = !horizontal;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        for (uint32_t i = 0; i < (Input::IsKeyHeld(Key::F1) ? 2 : 1); i++) {
            std::shared_ptr<Framebuffer> frameBuffer;

            if (i != 0) {
                frameBuffer = Framebuffer::Create({
                    .width = static_cast<uint32_t>(size.x), .height = static_cast<uint32_t>(size.y),
                    .samples = 1, .attachments = {
                        { FramebufferTextureFormat::RGBA8 }
                    }
                }); frameBuffer->Bind();
            }

            postProcessingShader->Bind();
            postProcessingShader->SetInt("textureScale", 1);
            postProcessingShader->SetInt("albedoMap", 0);
            postProcessingShader->SetInt("bloomBlur", 1);
            postProcessingShader->SetFloat("time", window->GetTime());
            postProcessingShader->SetFloat2("resolution", size);
            glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)), glm::vec3(1.0f));
            postProcessingShader->SetMat4("model", model);
            postProcessingShader->SetMat3("inverseModel", glm::transpose(glm::inverse(glm::mat3(model))));
            postProcessingShader->SetMat4("view", glm::mat4(1.0f));
            postProcessingShader->SetMat4("proj", glm::mat4(1.0f));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, postProcessingColorBuffers[0]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, pingPongColorBuffers[!horizontal]);
            RenderQuad();
            postProcessingShader->Unbind();

            if (i != 0) {
                uint32_t channels = 3;
                std::vector<uint8_t> pixels;
                frameBuffer->GetPixels(pixels, 0, channels);
                frameBuffer->Unbind();

                stbi_flip_vertically_on_write(true);
                stbi_write_png("screenshot.png", size.x, size.y, channels, pixels.data(), Framebuffer::CalculateStride(size.x, channels));
            }
        }

        ImGui::BeginChild("Render");
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((void*) (intptr_t) pingPongColorBuffers[!horizontal], wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::Image((void*) (intptr_t) depthMapDirectional, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::Image((void*) (intptr_t) ssaoColorBuffer, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::Image((void*) (intptr_t) gPosition, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::Image((void*) (intptr_t) gNormal, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::Image((void*) (intptr_t) gAlbedo, wsize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();

        ImGui::Checkbox("Enable SSAO", &enableSSAO);

        if (Input::IsKeyHeld(Key::GraveAccent)) {
            if (consoleKeyState)
                consoleKeyState = false, consoleState = !consoleState;
        } else {
            consoleKeyState = true;
        }

        if (consoleState) {
            ImGui::Begin("Console", &consoleState, ImGuiWindowFlags_NoResize);
            ImGui::SetWindowSize(ImVec2(size.x / 2, size.y / 2));

            for (std::string i : Logger::GetLogs())
                ImGui::Text(i.c_str());

            ImGui::End();
        }

        if (introState) {
            glEnable(GL_BLEND);
            glDisable(GL_DEPTH_TEST);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            if (backgroundTransparency != 0.0f) {
                basicShader->Bind();
                basicShader->SetInt("useAlbedoMap", 0);
                basicShader->SetInt("isTransparent", 1);
                basicShader->SetFloat("transparency", backgroundTransparency);
                basicShader->SetFloat3("albedoDefault", glm::vec3(0.0f, 0.0f, 0.0f));
                glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 0.0f));
                basicShader->SetMat4("model", model);
                basicShader->SetMat3("inverseModel", glm::transpose(glm::inverse(glm::mat3(model))));
                basicShader->SetMat4("view", glm::mat4(1.0f));
                basicShader->SetMat4("proj", glm::mat4(1.0f));
                RenderQuad();
                basicShader->Unbind();

                if (backgroundTransparency > 0.0f)
                    backgroundTransparency -= 0.6f * deltaTime;
                
                if (backgroundTransparency < 0.0f)
                    backgroundTransparency = 0.0f;
            }

            if (logoTransparency != 0.0f) {
                basicShader->Bind();
                basicShader->SetInt("textureScale", 1);
                basicShader->SetInt("useAlbedoMap", 1);
                basicShader->SetInt("isTransparent", 1);
                basicShader->SetFloat("transparency", logoTransparency);
                glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.3f / 3.0f * 2.0f, 0.12f / 3.0f * 2.0f, 0.0f));
                basicShader->SetMat4("model", model);
                basicShader->SetMat3("inverseModel", glm::transpose(glm::inverse(glm::mat3(model))));
                basicShader->SetMat4("view", glm::mat4(1.0f));
                basicShader->SetMat4("proj", glm::mat4(1.0f));
                basicShader->SetInt("albedoMap", 0);
                logoTexture->Bind();
                RenderQuad();
                basicShader->Unbind();

                if (logoTransparency > 0.0f)
                    logoTransparency -= 0.5f * deltaTime;
                
                if (logoTransparency < 0.0f)
                    logoTransparency = 0.0f;
            } else {
                introState = false;
            }

            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
        }
    }

    void SceneRenderer::RenderBackground() const {
        defaultShader->Bind();
        
        if (enableSkybox) {
            glDisable(GL_DEPTH_TEST);
            backgroundShader->Bind();
            backgroundShader->SetInt("environmentMap", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
            RenderCube();
            backgroundShader->Unbind();
            glEnable(GL_DEPTH_TEST);
        }

        defaultShader->Unbind();
    }

    void SceneRenderer::ReloadShaders() {
        shaderLibrary.Reload();
        defaultShader = shaderLibrary.Get("default");
        basicShader = shaderLibrary.Get("basic");
        depthCubeMapShader = shaderLibrary.Get("depthCubeMap");
        depthMapShader = shaderLibrary.Get("depthMap");
        postProcessingShader = shaderLibrary.Get("postProcessing");
        gBufferShader = shaderLibrary.Get("gBuffer");
        ssaoShader = shaderLibrary.Get("ssao");
        ssaoBlurShader = shaderLibrary.Get("ssaoBlur");
        bloomBlurShader = shaderLibrary.Get("bloomBlur");

        if (enableSkybox) {
            equirectangularToCubeMapShader = shaderLibrary.Get("equirectangularToCubeMap");
            irradianceShader = shaderLibrary.Get("irradiance");
            prefilterShader = shaderLibrary.Get("prefilter");
            brdfShader = shaderLibrary.Get("brdf");
            backgroundShader = shaderLibrary.Get("background");
        }
    }

    void SceneRenderer::Destroy() {
        if (enableSkybox)
            FreeSkybox();

        DestroyPostProcessing();
        DestroyShadowMaps();
        DestroyGBuffer();
        DestroySSAO();

        logoTexture = nullptr;
        defaultShader = nullptr, shaderLibrary.Free("default");
        basicShader = nullptr, shaderLibrary.Free("basic");
    }
}