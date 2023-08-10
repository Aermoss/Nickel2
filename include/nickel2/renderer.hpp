#pragma once

#include <vector>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include "vertexArray.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "utils.hpp"
#include "light.hpp"

namespace nickel2 {
    void renderSphere();
    void renderCube();
    void renederQuad();
    void destroyRenderer();

    class Camera;
    class Window;
    class Model;
    class Shader;
    class Texture;

    class Renderer {
        private:
            Window* window;
            std::vector <Model*> queue;
            uint32_t envCubemap, brdfLUTTexture, captureFBO, depthMapFBO, \
                captureRBO, depthCubemap, irradianceMap, prefilterMap;
            Texture *logoTexture, *hdrTexture;
            float logoTransparency, backgroundTransparency;

        public:
            bool useHDRTexture, intro;
            glm::vec2 depthMapSize;
            std::vector <Light> lights;
            Shader *shader, *basicShader, *depthShader, *equirectangularToCubemapShader, \
                *irradianceShader, *prefilterShader, *brdfShader, *backgroundShader;

            Renderer(Window* window, const std::string& hdrTexturePath = "", glm::ivec2 depthMapSize = glm::ivec2(8192, 8192));
            ~Renderer();

            void removeHDRTexture();
            void loadHDRTexture(const std::string& filePath);
            void reloadShaders();
            void bindTextures();
            void renderBackground();
            void updateLights();
            void submit(Model* model);
            void render(Camera* camera);
            void destroy();
    };
}