#pragma once

#include <vector>
#include <random>
#include <memory>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <glfw/glfw3native.h>

#include <imgui/imgui.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "VertexArray.hpp"
#include "UniformBuffer.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

#include "../Core/Input.hpp"
#include "../Core/Window.hpp"
#include "../Core/Utils.hpp"
#include "../Scene/Component.hpp"
#include "../Scene/Scene.hpp"

namespace Nickel2 {
    class Texture;
    class Framebuffer;
    class Scene;
    
    void RenderSphere();
    void RenderCube();
    void RenderQuad();

    struct BloomMip {
        glm::vec2 size;
        uint32_t texture;
    };

    class BloomFramebuffer {
        private:
            std::shared_ptr<Framebuffer> framebuffer;
            std::vector<BloomMip> mipChain;

        public:
            BloomFramebuffer(uint32_t windowWidth, uint32_t windowHeight, uint32_t mipChainSize);
            ~BloomFramebuffer();

            void Bind() const;
            void Unbind() const;

            const std::vector<BloomMip>& GetMipChain() const { return mipChain; }
    };

    class BloomRenderer {
        private:
            void RenderDownsamples(uint32_t sourceTexture);
            void RenderUpsamples(float filterRadius);

            bool karisAverage = false;

            glm::vec2 viewportSize;
            std::shared_ptr<Shader> downsampleShader, upsampleShader;
            std::shared_ptr<BloomFramebuffer> framebuffer;
            uint32_t mipChainSize;

        public:
            BloomRenderer(uint32_t windowWidth, uint32_t windowHeight, uint32_t mipChainSize = 6);
            ~BloomRenderer();

            void Resize(uint32_t windowWidth, uint32_t windowHeight);
            uint32_t GetBloomTexture() const { return framebuffer->GetMipChain()[0].texture; }
            void RenderBloomTexture(uint32_t sourceTexture, float filterRadius);
            void ReloadShaders();

            friend class SceneRenderer;
    };

    class SceneRenderer {
        private:
            Window* window;
            std::vector<Mesh*> queue;
            std::shared_ptr<Texture> logoTexture, hdrTexture;
            std::shared_ptr<BloomRenderer> bloomRenderer;
            std::vector<glm::vec3> ssaoKernel;

            std::shared_ptr<Framebuffer> depthMapDirectionalFramebuffer;

            uint32_t gPosition, gAlbedo, gNormal, gBuffer, envCubeMap, brdfLUT, captureFramebuffer, captureRenderbuffer, depthRenderbuffer, \
                irradianceMap, prefilterMap, depthMapPointFramebuffer, depthMapPoint, depthMapDirectional, ssaoFramebuffer, ssaoBlurFramebuffer, \
                ssaoColorBuffer, ssaoColorBufferBlur, noiseTexture, postProcessingFramebuffer, postProcessingRenderbuffer, postProcessingColorBuffers[2];

            float bloomFilterRadius = 0.005f, \
                logoTransparency = 1.0f, backgroundTransparency = 1.0f;

            bool introState = true, consoleState = false, consoleKeyState = false, \
                enableSkybox = false, enableSSAO = true, enableGBuffer = true;

            void InitializeShadowMaps();
            void TerminateShadowMaps();

            void InitializeGBuffer();
            void TerminateGBuffer();

            void InitializeSSAO();
            void TerminateSSAO();

            void InitializePostProcessing();
            void TerminatePostProcessing();

        public:
            glm::vec4 clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
            glm::vec2 windowSize, depthMapSize = glm::vec2(8192.0f, 8192.0f);
            ShaderLibrary shaderLibrary;

            std::shared_ptr<Shader> defaultShader, basicShader, depthCubeMapShader, depthMapShader, equirectangularToCubeMapShader, \
                postProcessingShader, irradianceShader, prefilterShader, brdfShader, backgroundShader, gBufferShader, ssaoShader, ssaoBlurShader;

            void BindPostProcessingFramebuffer();
            void UnbindPostProcessingFramebuffer();

            void FreeSkybox();
            void LoadSkybox(const std::string& filePath);
            uint32_t GetSkyboxTexture() { return envCubeMap; }

            void UpdatePointLights(std::vector<Light>& lights);
            void UpdateShadowMaps(std::vector<Light>& lights);
            void UpdateShadowMaps(Scene* scene, bool updateQueue = true);
            
            SceneRenderer(Window* window, const std::string& skyboxPath = "");
            ~SceneRenderer();

            void Submit(Mesh* model);
            void Render(Scene* scene, float deltaTime, float shadownUpdateInterval = 0.0f, bool updateCamera = true, bool renderBackground = true);
            void RenderBackground() const;
            void ReloadShaders();
    };
}