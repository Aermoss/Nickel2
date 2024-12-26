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

            for (uint32_t x = 0; x <= X_SEGMENTS; x++) {
                for (uint32_t y = 0; y <= Y_SEGMENTS; y++) {
                    float xSegment = (float) x / (float) X_SEGMENTS;
                    float ySegment = (float) y / (float) Y_SEGMENTS;
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

    BloomFramebuffer::BloomFramebuffer(uint32_t windowWidth, uint32_t windowHeight, uint32_t mipChainSize) {
        glm::vec2 mipSize(windowWidth, windowHeight); {
            framebuffer = Framebuffer::Create({
                .width = static_cast<uint32_t>(mipSize.x), .height = static_cast<uint32_t>(mipSize.y), .samples = 1,
                .attachments = { FramebufferTextureFormat::RGBA16F }
            });

            BloomMip mip = { mipSize, framebuffer->GetColorAttachment() };
            mipChain.emplace_back(mip);
        }

        for (uint32_t i = 1; i < mipChainSize; i++) {
            mipSize *= 0.5f;
            BloomMip mip = { mipSize, 0 };
            glGenTextures(1, &mip.texture);
            glBindTexture(GL_TEXTURE_2D, mip.texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, static_cast<uint32_t>(mipSize.x), static_cast<uint32_t>(mipSize.y), 0, GL_RGBA, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            mipChain.emplace_back(mip);
        }
    }

    BloomFramebuffer::~BloomFramebuffer() {
        for (uint64_t i = 1; i < mipChain.size(); i++)
            glDeleteTextures(1, &mipChain[i].texture);

        framebuffer = nullptr;
    }

    void BloomFramebuffer::Bind() const {
        framebuffer->Bind();
    }

    void BloomFramebuffer::Unbind() const {
        framebuffer->Unbind();
    }

    BloomRenderer::BloomRenderer(uint32_t windowWidth, uint32_t windowHeight, uint32_t mipChainSize) : mipChainSize(mipChainSize) {
        framebuffer = std::make_shared<BloomFramebuffer>(windowWidth, windowHeight, mipChainSize);
        viewportSize = glm::vec2(windowWidth, windowHeight);
        this->ReloadShaders();
    }

    BloomRenderer::~BloomRenderer() {
        framebuffer = nullptr, downsampleShader = nullptr, upsampleShader = nullptr;
    }

    void BloomRenderer::Resize(uint32_t windowWidth, uint32_t windowHeight) {
        framebuffer = std::make_shared<BloomFramebuffer>(windowWidth, windowHeight, mipChainSize);
        viewportSize = glm::vec2(windowWidth, windowHeight);
    }

    void BloomRenderer::RenderDownsamples(uint32_t sourceTexture) {
        const std::vector<BloomMip>& mipChain = framebuffer->GetMipChain();

        downsampleShader->Bind();
        downsampleShader->SetInt("sourceTexture", 0);
        glm::vec2 texelSize = 1.0f / viewportSize;

        glDisable(GL_BLEND);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sourceTexture);

        for (uint64_t i = 0; i < mipChain.size(); i++) {
            const BloomMip& mip = mipChain[i];
            RenderCommand::SetViewport(0, 0, mip.size.x, mip.size.y);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mip.texture, 0);
            downsampleShader->SetInt("karisAverage", (karisAverage && i == 0) ? 1 : 0);
            downsampleShader->SetFloat2("texelSize", texelSize);
            RenderQuad();
            glBindTexture(GL_TEXTURE_2D, mip.texture);
            texelSize *= 2.0f;
        }

        downsampleShader->Unbind();
    }

    void BloomRenderer::RenderUpsamples(float filterRadius) {
        const std::vector<BloomMip>& mipChain = framebuffer->GetMipChain();

        upsampleShader->Bind();
        upsampleShader->SetFloat("filterRadius", filterRadius);
        upsampleShader->SetInt("sourceTexture", 0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glBlendEquation(GL_FUNC_ADD);

        for (uint32_t i = static_cast<uint32_t>(mipChain.size()) - 1; i > 0; i--) {
            const BloomMip& mip = mipChain[i];
            const BloomMip& nextMip = mipChain[i - 1];

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mip.texture);
            RenderCommand::SetViewport(0, 0, nextMip.size.x, nextMip.size.y);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, nextMip.texture, 0);
            RenderQuad();
        }

        glDisable(GL_BLEND);
        upsampleShader->Unbind();
    }

    void BloomRenderer::RenderBloomTexture(uint32_t sourceTexture, float filterRadius) {
        framebuffer->Bind();
        this->RenderDownsamples(sourceTexture);
        this->RenderUpsamples(filterRadius);
        framebuffer->Unbind();

        RenderCommand::SetViewport(0, 0, viewportSize.x, viewportSize.y);
    }

    void BloomRenderer::ReloadShaders() {
        downsampleShader = Shader::Create("downsample", {
            { ShaderStage::Vertex, Utils::ReadFile("shaders/sampler.vert") },
            { ShaderStage::Fragment, Utils::ReadFile("shaders/downsample.frag") }
        });

        upsampleShader = Shader::Create("upsample", {
            { ShaderStage::Vertex, Utils::ReadFile("shaders/sampler.vert") },
            { ShaderStage::Fragment, Utils::ReadFile("shaders/upsample.frag") }
        });
    }

    void SceneRenderer::InitializeShadowMaps() {
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

        depthMapPointFramebuffer = Framebuffer::Create({
            .width = static_cast<uint32_t>(depthMapSize.x), .height = static_cast<uint32_t>(depthMapSize.y),
            .samples = 1, .attachments = {}, .checkStatus = false
        });

        glGenTextures(1, &depthMapPoint);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapPoint);

        for (uint32_t i = 0; i < 6; i++)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, depthMapSize.x, depthMapSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        depthMapPointFramebuffer->Bind();
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMapPoint, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        depthMapPointFramebuffer->Unbind();

        depthMapDirectionalFramebuffer = Framebuffer::Create({
            .width = static_cast<uint32_t>(depthMapSize.x), .height = static_cast<uint32_t>(depthMapSize.x), .samples = 1,
            .attachments = { FramebufferTextureFormat::DEPTH24STENCIL8 }
        });
    }

    void SceneRenderer::TerminateShadowMaps() {
        glDeleteTextures(1, &depthMapPoint);
        depthMapPointFramebuffer = nullptr, depthMapDirectionalFramebuffer = nullptr;
        depthCubeMapShader = nullptr, shaderLibrary.Free("depthCubeMap");
        depthMapShader = nullptr, shaderLibrary.Free("depthMap");
    }

    void SceneRenderer::InitializeGBuffer() {
        gBufferShader = shaderLibrary.Load("gBuffer", {
            { ShaderStage::Vertex, "shaders/gBuffer.vert" },
            { ShaderStage::Fragment, "shaders/gBuffer.frag" }
        });

        gBuffer = Framebuffer::Create({
            .width = static_cast<uint32_t>(windowSize.x), .height = static_cast<uint32_t>(windowSize.y), .samples = 1,
            .attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA }
        }), enableGBuffer = true;

        gBuffer->Bind();
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowSize.x, windowSize.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
        gBuffer->Unbind();
    }

    void SceneRenderer::TerminateGBuffer() {
        gBuffer = nullptr, enableGBuffer = false;
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        gBufferShader = nullptr, shaderLibrary.Free("gBuffer");
    }

    void SceneRenderer::InitializeSSAO() {
        ssaoShader = shaderLibrary.Load("ssao", {
            { ShaderStage::Vertex, "shaders/sampler.vert" },
            { ShaderStage::Fragment, "shaders/ssao.frag" }
        });

        ssaoBlurShader = shaderLibrary.Load("ssaoBlur", {
            { ShaderStage::Vertex, "shaders/sampler.vert" },
            { ShaderStage::Fragment, "shaders/ssaoBlur.frag" }
        });

        ssaoFramebuffer = Framebuffer::Create({
            .width = static_cast<uint32_t>(windowSize.x), .height = static_cast<uint32_t>(windowSize.y), .samples = 1,
            .attachments = { FramebufferTextureFormat::RED32F }
        });

        ssaoBlurFramebuffer = Framebuffer::Create({
            .width = static_cast<uint32_t>(windowSize.x), .height = static_cast<uint32_t>(windowSize.y), .samples = 1,
            .attachments = { FramebufferTextureFormat::RED32F }
        });

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

        for (uint32_t i = 0; i < 16; i++)
            ssaoNoise.push_back(glm::vec3(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, 0.0f));

        glGenTextures(1, &noiseTexture);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        enableSSAO = true;
    }

    void SceneRenderer::TerminateSSAO() {
        glDeleteTextures(1, &noiseTexture);
        ssaoFramebuffer = nullptr, ssaoBlurFramebuffer = nullptr;
        ssaoShader = nullptr, shaderLibrary.Free("ssao");
        ssaoBlurShader = nullptr, shaderLibrary.Free("ssaoBlur");
        enableSSAO = false;
    }

    void SceneRenderer::InitializePostProcessing() {
        glm::vec2 size = window->GetSize();

        postProcessingShader = shaderLibrary.Load("postProcessing", {
            { ShaderStage::Vertex, "shaders/default.vert" },
            { ShaderStage::Fragment, "shaders/postProcessing.frag" }
        });

        postProcessingFramebuffer = Framebuffer::Create({
            .width = static_cast<uint32_t>(size.x), .height = static_cast<uint32_t>(size.y), .samples = 1,
            .attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA16F }
        });

        postProcessingFramebuffer->Bind();
        glGenRenderbuffers(1, &postProcessingRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, postProcessingRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, postProcessingRenderbuffer);
        postProcessingFramebuffer->Unbind();
    }

    void SceneRenderer::TerminatePostProcessing() {
        postProcessingFramebuffer = nullptr;
        glDeleteRenderbuffers(1, &postProcessingRenderbuffer);
        postProcessingShader = nullptr, shaderLibrary.Free("postProcessing");
    }

    void SceneRenderer::BindPostProcessingFramebuffer() {
        postProcessingFramebuffer->Bind();
    }

    void SceneRenderer::UnbindPostProcessingFramebuffer() {
        postProcessingFramebuffer->Unbind();
    }

    void SceneRenderer::FreeSkybox() {
        if (!enableSkybox) return;
        enableSkybox = false;

        glDeleteTextures(1, &envCubeMap);
        glDeleteTextures(1, &brdfLUT);
        glDeleteTextures(1, &prefilterMap);
        glDeleteRenderbuffers(1, &captureRenderbuffer);

        hdrTexture = nullptr, captureFramebuffer = nullptr;
        equirectangularToCubeMapShader = nullptr, shaderLibrary.Free("equirectangularToCubeMap");
        irradianceShader = nullptr, shaderLibrary.Free("irradiance");
        prefilterShader = nullptr, shaderLibrary.Free("prefilter");
        brdfShader = nullptr, shaderLibrary.Free("brdf");
        backgroundShader = nullptr, shaderLibrary.Free("background");
    }

    void SceneRenderer::LoadSkybox(const std::string& filePath) {
        if (enableSkybox) FreeSkybox();
        enableSkybox = true;

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
            { ShaderStage::Vertex, "shaders/sampler.vert" },
            { ShaderStage::Fragment, "shaders/brdf.frag" }
        });

        backgroundShader = shaderLibrary.Load("background", {
            { ShaderStage::Vertex, "shaders/background.vert" },
            { ShaderStage::Fragment, "shaders/background.frag" }
        });

        captureFramebuffer = Framebuffer::Create({
            .width = 512, .height = 512, .samples = 1,
            .attachments = {}, .checkStatus = false
        });

        captureFramebuffer->Bind();

        glGenRenderbuffers(1, &captureRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRenderbuffer);

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

        RenderCommand::SetViewport(0, 0, 512, 512);

        for (uint32_t i = 0; i < 6; ++i) {
            equirectangularToCubeMapShader->SetMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubeMap, 0);
            RenderCommand::Clear();
            RenderCube();
        }

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
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

        irradianceShader->Bind();
        irradianceShader->SetInt("environmentMap", 0);
        irradianceShader->SetMat4("proj", captureProjection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
        RenderCommand::SetViewport(0, 0, 32, 32);

        for (uint32_t i = 0; i < 6; ++i) {
            irradianceShader->SetMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
            RenderCommand::Clear();
            RenderCube();
        }

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

        uint32_t maxMipLevels = 5;

        for (uint32_t mip = 0; mip < maxMipLevels; ++mip) {
            uint32_t mipWidth  = static_cast<uint32_t>(128 * std::pow(0.5f, mip));
            uint32_t mipHeight = static_cast<uint32_t>(128 * std::pow(0.5f, mip));
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            RenderCommand::SetViewport(0, 0, mipWidth, mipHeight);

            float roughness = (float) mip / (float) (maxMipLevels - 1);
            prefilterShader->SetFloat("roughness", roughness);

            for (uint32_t i = 0; i < 6; ++i) {
                prefilterShader->SetMat4("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);
                RenderCommand::Clear();
                RenderCube();
            }
        }

        glGenTextures(1, &brdfLUT);
        glBindTexture(GL_TEXTURE_2D, brdfLUT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUT, 0);

        RenderCommand::SetViewport(0, 0, 512, 512);
        brdfShader->Bind();
        RenderCommand::Clear();
        RenderQuad();
        brdfShader->Unbind();

        captureFramebuffer->Unbind();
        glm::vec2 size = window->GetSize();
        RenderCommand::SetViewport(0, 0, size.x, size.y);
    }

    void SceneRenderer::UpdatePointLights(std::vector<PointLightComponent*> lights) {
        std::vector<glm::vec4> colors;
        std::vector<glm::vec3> positions;

        for (uint32_t i = 0; i < lights.size(); i++)
            positions.push_back(lights[i]->position), colors.push_back(glm::vec4(lights[i]->color, lights[i]->brightness));

        defaultShader->Bind();
        defaultShader->SetInt("pointLightCount", lights.size());
        defaultShader->SetFloatArray3("pointLightPositions", positions.data(), lights.size());
        defaultShader->SetFloatArray4("pointLightColors", colors.data(), lights.size());
        defaultShader->Unbind();
    }

    void SceneRenderer::UpdatePointShadowMap(glm::vec3 position) {
        glm::mat4 proj = glm::perspective(glm::radians(90.0f), depthMapSize.x / depthMapSize.y, 0.1f, 1000.0f);

        std::vector<glm::mat4> shadowTransforms {
            proj * glm::lookAt(position, position + (glm::vec3) {  1.0f,  0.0f,  0.0f }, { 0.0f, -1.0f,  0.0f }),
            proj * glm::lookAt(position, position + (glm::vec3) { -1.0f,  0.0f,  0.0f }, { 0.0f, -1.0f,  0.0f }),
            proj * glm::lookAt(position, position + (glm::vec3) {  0.0f,  1.0f,  0.0f }, { 0.0f,  0.0f,  1.0f }),
            proj * glm::lookAt(position, position + (glm::vec3) {  0.0f, -1.0f,  0.0f }, { 0.0f,  0.0f, -1.0f }),
            proj * glm::lookAt(position, position + (glm::vec3) {  0.0f,  0.0f,  1.0f }, { 0.0f, -1.0f,  0.0f }),
            proj * glm::lookAt(position, position + (glm::vec3) {  0.0f,  0.0f, -1.0f }, { 0.0f, -1.0f,  0.0f })
        };

        RenderCommand::SetViewport(0, 0, depthMapSize.x, depthMapSize.y);
        depthMapPointFramebuffer->Bind();
        depthCubeMapShader->Bind();
        RenderCommand::Clear();

        for (uint32_t i = 0; i < shadowTransforms.size(); i++)
            depthCubeMapShader->SetMat4(("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowTransforms[i]);

        depthCubeMapShader->SetFloat3("lightPosition", position);
        depthCubeMapShader->Unbind();

        for (uint32_t i = 0; i < queue.size(); i++)
            queue[i]->Render(depthCubeMapShader, false, true);

        depthMapPointFramebuffer->Unbind();
        glm::vec2 size = window->GetSize();
        RenderCommand::SetViewport(0, 0, size.x, size.y);
    }

    void SceneRenderer::UpdateDirectionalShadowMap(Scene* scene) {
        Entity* camera = scene->GetPrimaryCameraEntity();
        std::vector<Entity*> entities = scene->GetEntitiesWith<DirectionalLightComponent>();
        NK_CORE_ASSERT(entities.size() <= 1, "Only one directional light is supported");

        defaultShader->Bind();
        defaultShader->SetInt("enableDirectionalLight", entities.size() > 0 ? 1 : 0);
        defaultShader->Unbind();

        for (Entity* entity : entities) {
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

            RenderCommand::SetViewport(0, 0, depthMapSize.x, depthMapSize.y);
            depthMapDirectionalFramebuffer->Bind();
            RenderCommand::Clear();
            
            for (uint32_t i = 0; i < queue.size(); i++)
                queue[i]->Render(depthMapShader, false, true);

            depthMapDirectionalFramebuffer->Unbind();
            glm::vec2 size = window->GetSize();
            RenderCommand::SetViewport(0, 0, size.x, size.y);

            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, depthMapDirectionalFramebuffer->GetDepthAttachment());
        }
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

        InitializePostProcessing();
        InitializeShadowMaps();
        InitializeGBuffer();
        InitializeSSAO();

        bloomRenderer = std::make_shared<BloomRenderer>(windowSize.x, windowSize.y);

        if (!skyboxPath.empty())
            LoadSkybox(skyboxPath);
    }

    SceneRenderer::~SceneRenderer() {
        if (enableSkybox)
            FreeSkybox();

        TerminatePostProcessing();
        TerminateShadowMaps();
        TerminateGBuffer();
        TerminateSSAO();

        logoTexture = nullptr, bloomRenderer = nullptr;
        defaultShader = nullptr, shaderLibrary.Free("default");
        basicShader = nullptr, shaderLibrary.Free("basic");
    }

    void SceneRenderer::Submit(Mesh* model) {
        queue.push_back(model);
    }

    void SceneRenderer::Render(Scene* scene, float deltaTime, float shadownUpdateInterval, bool updateCamera, bool renderBackground) {
        Camera* camera = scene->GetPrimaryCamera();
        this->UpdatePointLights(scene->GetInstancesOf<PointLightComponent>());

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
            TerminatePostProcessing(), InitializePostProcessing();

            if (enableGBuffer)
                TerminateGBuffer(), InitializeGBuffer();

            if (enableSSAO)
                TerminateSSAO(), InitializeSSAO();

            bloomRenderer->Resize(size.x, size.y);
        }

        for (Entity* entity : scene->GetEntitiesWith<MeshComponent>())
            this->Submit(entity->GetComponent<MeshComponent>().mesh);

        static float shadowUpdateCooldown = 0.0f;

        if (shadowUpdateCooldown <= 0.0f) {
            this->UpdateDirectionalShadowMap(scene);
            std::vector<Entity*> entities = scene->GetEntitiesWith<PointLightComponent>();

            if (!entities.empty()) {
                glm::vec3 translation, scale; glm::quat rotation;
                glm::mat4 transform = entities[0]->GetComponent<TransformComponent>().GetWorldTransform();
                Math::DecomposeTransform(transform, translation, rotation, scale);
                this->UpdatePointShadowMap(translation);
            }

            shadowUpdateCooldown = shadownUpdateInterval;
        } shadowUpdateCooldown -= deltaTime;

        if (updateCamera) {
            camera->UpdateMatrices(defaultShader);
            camera->UpdateMatrices(basicShader);

            if (enableSkybox && renderBackground)
                camera->UpdateMatrices(backgroundShader);
        }

        if (enableGBuffer) {
            gBuffer->Bind();
            RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            RenderCommand::Clear();
            camera->UpdateMatrices(gBufferShader);

            for (uint32_t i = 0; i < queue.size(); i++)
                queue[i]->Render(gBufferShader, false, true);

            RenderCommand::SetClearColor(clearColor);
            gBuffer->Unbind();
        }

        if (enableSSAO) {
            if (!enableGBuffer) {
                Nickel2::Logger::Log(Nickel2::Logger::Level::Error, "SceneRenderer", "SSAO requires G-buffer to be enabled.");
                return;
            }

            ssaoFramebuffer->Bind();
            RenderCommand::Clear();

            camera->UpdateMatrices(ssaoShader);
            
            ssaoShader->Bind();
            ssaoShader->SetInt("gPosition", 0);
            ssaoShader->SetInt("gNormal", 1);
            ssaoShader->SetInt("texNoise", 2);
            ssaoShader->SetFloat2("windowSize", size);
            ssaoShader->SetFloatArray3("samples", ssaoKernel.data(), ssaoKernel.size());

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gBuffer->GetColorAttachment(0));
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gBuffer->GetColorAttachment(1));
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, noiseTexture);
            RenderQuad();
            ssaoShader->Unbind();
            ssaoFramebuffer->Unbind();

            ssaoBlurFramebuffer->Bind();
            RenderCommand::Clear();
            ssaoBlurShader->Bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ssaoFramebuffer->GetColorAttachment());
            RenderQuad();
            ssaoBlurShader->Unbind();
            ssaoBlurFramebuffer->Unbind();
        }

        BindPostProcessingFramebuffer();

        if (enableSkybox && renderBackground) {
            RenderCommand::Clear();
            this->RenderBackground();
        }

        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapPoint);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, depthMapDirectionalFramebuffer->GetDepthAttachment());
        glActiveTexture(GL_TEXTURE11);
        glBindTexture(GL_TEXTURE_2D, gBuffer->GetColorAttachment(0));
        glActiveTexture(GL_TEXTURE12);
        glBindTexture(GL_TEXTURE_2D, gBuffer->GetColorAttachment(1));
        glActiveTexture(GL_TEXTURE13);
        glBindTexture(GL_TEXTURE_2D, gBuffer->GetColorAttachment(2));
        glActiveTexture(GL_TEXTURE14);
        glBindTexture(GL_TEXTURE_2D, ssaoBlurFramebuffer->GetColorAttachment());

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

        bloomRenderer->RenderBloomTexture(postProcessingFramebuffer->GetColorAttachment(1), bloomFilterRadius);

        for (uint32_t i = 0; i < (Input::IsKeyHeld(Key::F1) ? 2 : 1); i++) {
            std::shared_ptr<Framebuffer> frameBuffer;

            if (i != 0) {
                frameBuffer = Framebuffer::Create({
                    .width = static_cast<uint32_t>(size.x), .height = static_cast<uint32_t>(size.y),
                    .samples = 1, .attachments = { FramebufferTextureFormat::RGBA }
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
            glBindTexture(GL_TEXTURE_2D, postProcessingFramebuffer->GetColorAttachment(0));
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, bloomRenderer->GetBloomTexture());
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

        ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

        if (ImGui::TreeNode("Bloom")) {
            ImGui::SliderFloat("Bloom Filter Radius", &bloomFilterRadius, 0.001f, 0.015f);
            ImGui::Image((void*) (intptr_t) postProcessingFramebuffer->GetColorAttachment(1), { 256.0f, 256.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            ImGui::Image((void*) (intptr_t) bloomRenderer->GetBloomTexture(), { 256.0f, 256.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Shadows")) {
            ImGui::Image((void*) (intptr_t) depthMapDirectionalFramebuffer->GetDepthAttachment(), { 256.0f, 256.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("G-Buffer")) {
            ImGui::Image((void*) (intptr_t) gBuffer->GetColorAttachment(0), { 256.0f, 256.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            ImGui::Image((void*) (intptr_t) gBuffer->GetColorAttachment(1), { 256.0f, 256.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            ImGui::Image((void*) (intptr_t) gBuffer->GetColorAttachment(2), { 256.0f, 256.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("SSAO")) {
            ImGui::Checkbox("Enable SSAO", &enableSSAO);
            ImGui::Image((void*) (intptr_t) ssaoFramebuffer->GetColorAttachment(), { 256.0f, 256.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            ImGui::Image((void*) (intptr_t) ssaoBlurFramebuffer->GetColorAttachment(), { 256.0f, 256.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f });
            ImGui::TreePop();
        }

        ImGui::SetWindowPos({ 10.0f, 10.0f });
        ImGui::End();

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
        bloomRenderer->ReloadShaders();
        
        shaderLibrary.Reload();
        defaultShader = shaderLibrary.Get("default");
        basicShader = shaderLibrary.Get("basic");
        depthCubeMapShader = shaderLibrary.Get("depthCubeMap");
        depthMapShader = shaderLibrary.Get("depthMap");
        postProcessingShader = shaderLibrary.Get("postProcessing");
        gBufferShader = shaderLibrary.Get("gBuffer");
        ssaoShader = shaderLibrary.Get("ssao");
        ssaoBlurShader = shaderLibrary.Get("ssaoBlur");

        if (enableSkybox) {
            equirectangularToCubeMapShader = shaderLibrary.Get("equirectangularToCubeMap");
            irradianceShader = shaderLibrary.Get("irradiance");
            prefilterShader = shaderLibrary.Get("prefilter");
            brdfShader = shaderLibrary.Get("brdf");
            backgroundShader = shaderLibrary.Get("background");
        }
    }
}