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
#define GLFW_NATIVE_INCLUDE_NONE
#include <glfw/glfw3native.h>

#include <imgui/imgui.h>
#include <stb/stb_image.h>
#include <rvr/rvr.hpp>

#include "VertexArray.hpp"
#include "Buffer.hpp"
#include "Shader.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"

#include "../Core/Window.hpp"
#include "../Core/Utils.hpp"
#include "../Scene/Component.hpp"
#include "../Scene/Scene.hpp"

namespace Nickel2 {
    class Texture;
    class Scene;
    
    void renderSphere();
    void renderCube();
    void renderQuad();
    void destroyRenderer();

    class Renderer {
        private:
            Window* window;
            std::vector<Mesh*> queue;
            std::vector<glm::vec3> ssaoKernel;
            float logoTransparency, backgroundTransparency;
            uint32_t envCubemap, brdfLUT, captureFBO, captureRBO, gPosition, gAlbedo, gNormal, gBuffer, depthRBO, \
                irradianceMap, prefilterMap, depthMapPointFBO, depthMapPoint, depthMapDirectionalFBO, depthMapDirectional, \
                ssaoFBO, ssaoBlurFBO, ssaoColorBuffer, ssaoColorBufferBlur, noiseTexture;
            Texture *logoTexture, *hdrTexture;
            glm::mat4 directionalLightSpaceMatrix;
            bool intro, consoleState, consoleKeyState, \
                useHDRTexture, enableSSAO, enableGBuffer;

        public:
            glm::vec2 depthMapSize;
            int32_t windowWidth, windowHeight;
            bool imguiShouldRender, imguiFrameState;
            Shader *shader, *basicShader, *depthCubemapShader, *depthMapShader, *equirectangularToCubemapShader, \
                *irradianceShader, *prefilterShader, *brdfShader, *backgroundShader, *gBufferShader, *ssaoShader, *ssaoBlurShader;

            uint32_t getEnvironmentCubeMap();

            void removeHDRTexture();
            void loadHDRTexture(const std::string& filePath);
            void reloadShaders();
            void bindTextures();
            void renderBackground();
            void setupShadowMaps();
            void destroyShadowMaps();
            void updatePointLights(std::vector<Light>& lights);
            void updateShadowMaps(std::vector<Light>& lights);
            void updateShadowMaps(Scene* scene, bool updateQueue = true);
            void setupGBuffer();
            void destroyGBuffer();
            void setupSSAO();
            void destroySSAO();
            
            Renderer(Window* window, const std::string& hdrTexturePath = "", glm::ivec2 depthMapSize = glm::ivec2(8192, 8192));
            ~Renderer();

            void imGuiNewFrame(bool setContext = true);
            void imGuiEndFrame();
            void imGuiRender();
            void submit(Mesh* model);
            void render(Scene* scene, bool updateShadowMaps = true, bool updateCamera = true, bool renderBackground = true);
            void destroy();
    };
}