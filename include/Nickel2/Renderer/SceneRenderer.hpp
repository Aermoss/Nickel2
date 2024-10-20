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
#include "Light.hpp"
#include "Mesh.hpp"

#include "../Core/Input.hpp"
#include "../Core/Window.hpp"
#include "../Core/Utils.hpp"
#include "../Scene/Component.hpp"
#include "../Scene/Scene.hpp"

namespace Nickel2 {
    class Texture;
    class Scene;
    
    void RenderSphere();
    void RenderCube();
    void RenderQuad();
    void DestroyRenderer();

    class SceneRenderer {
        private:
            Window* window;
            std::vector<Mesh*> queue;
            std::shared_ptr<Texture> logoTexture, hdrTexture;
            std::vector<glm::vec3> ssaoKernel;
            
            uint32_t gPosition, gAlbedo, gNormal, gBuffer, envCubeMap, brdfLUT, captureFrameBuffer, captureRenderBuffer, depthRenderBuffer, \
                irradianceMap, prefilterMap, depthMapPointFrameBuffer, depthMapPoint, depthMapDirectionalFrameBuffer, depthMapDirectional, \
                    ssaoFrameBuffer, ssaoBlurFrameBuffer, ssaoColorBuffer, ssaoColorBufferBlur, noiseTexture, postProcessingFrameBuffer, \
                        postProcessingRenderBuffer, postProcessingColorBuffers[2], pingPongFrameBuffers[2], pingPongColorBuffers[2];

            float logoTransparency = 1.0f, backgroundTransparency = 1.0f;
            bool introState = true, consoleState = false, consoleKeyState = false, \
                enableSkybox = false, enableSSAO = true, enableGBuffer = true;

            void SetupShadowMaps();
            void DestroyShadowMaps();

            void SetupGBuffer();
            void DestroyGBuffer();

            void SetupSSAO();
            void DestroySSAO();

            void SetupPostProcessing();
            void DestroyPostProcessing();

        public:
            glm::vec4 clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
            glm::vec2 windowSize, depthMapSize = glm::vec2(8192.0f, 8192.0f);
            ShaderLibrary shaderLibrary;

            std::shared_ptr<Shader> defaultShader, basicShader, depthCubeMapShader, depthMapShader, equirectangularToCubeMapShader, bloomBlurShader, \
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
            ~SceneRenderer() { this->Destroy(); }

            void Submit(Mesh* model);
            void Render(Scene* scene, float deltaTime, float shadownUpdateInterval = 0.0f, bool updateCamera = true, bool renderBackground = true);
            void RenderBackground() const;
            void ReloadShaders();
            void Destroy();
    };
}