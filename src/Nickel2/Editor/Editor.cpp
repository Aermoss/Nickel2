#if 1
#include <Nickel2/nkpch.hpp>
#include <Nickel2/Nickel2.hpp>

#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <vector>
#include <windows.h>
#include <dwmapi.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/loader.h>
#include <mono/metadata/object.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/threads.h>

bool decomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale) {
    using T = float;

    glm::mat4 localMatrix(transform);

    if (glm::epsilonEqual(localMatrix[3][3], static_cast<float>(0), glm::epsilon<T>()))
        return false;

    if (glm::epsilonNotEqual(localMatrix[0][3], static_cast<T>(0), glm::epsilon<T>())
    || glm::epsilonNotEqual(localMatrix[1][3], static_cast<T>(0), glm::epsilon<T>())
    || glm::epsilonNotEqual(localMatrix[2][3], static_cast<T>(0), glm::epsilon<T>())) {
        localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<T>(0);
        localMatrix[3][3] = static_cast<T>(1);
    }

    translation = glm::vec3(localMatrix[3]);
    localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

    glm::vec3 row[3];

    for (glm::length_t i = 0; i < 3; ++i)
        for (glm::length_t j = 0; j < 3; ++j)
            row[i][j] = localMatrix[i][j];

    scale.x = glm::length(row[0]);
    row[0] = glm::detail::scale(row[0], static_cast<T>(1));
    scale.y = glm::length(row[1]);
    row[1] = glm::detail::scale(row[1], static_cast<T>(1));
    scale.z = glm::length(row[2]);
    row[2] = glm::detail::scale(row[2], static_cast<T>(1));

    rotation.y = glm::asin(-row[0][2]);

    if (glm::cos(rotation.y) != 0) {
        rotation.x = atan2(row[1][2], row[2][2]);
        rotation.z = atan2(row[0][1], row[0][0]);
    } else {
        rotation.x = atan2(-row[2][0], row[1][1]);
        rotation.z = 0;
    } return true;
}

#define NICKEL2_ADD_INTERNAL_CALL(func) mono_add_internal_call("Nickel2.InternalCalls::" #func, (const void*) func)

std::string monoStringToString(MonoString* string) {
    char* cStr = mono_string_to_utf8(string);
    std::string str(cStr);
    mono_free(cStr);
    return str;
}

MonoAssembly* loadMonoAssembly(const char* assemblyPath) {
    MonoImageOpenStatus status;
    Nickel2::ext::ScopedBuffer fileData = Nickel2::readFileBinary(assemblyPath);
    MonoImage* image = mono_image_open_from_data_full(fileData.as<char>(), fileData.size(), 1, &status, 0);

    if (status != MONO_IMAGE_OK) {
        const char* errorMessage = mono_image_strerror(status);
        std::cout << "failed to load assembly: " << errorMessage << std::endl;
        return nullptr;
    }

    MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath, &status, 0);
    mono_image_close(image);
    return assembly;
}

static bool Input_IsKeyDown(int32_t keycode) {
    return Nickel2::getContext()->getCurentWindow()->input->getKey(keycode);
}

class FirstPersonCamera : public Nickel2::Entity {
    private:
        Nickel2::Window* window;

    public:
        float sensitivity, normalSpeed, sprintSpeed;
        float targetFov, lastScrollY, realYaw, realPitch, interpolatedYaw, interpolatedPitch;

        FirstPersonCamera(Nickel2::Scene* scene, Nickel2::Window* window, float fov, float sensitivity, float near, float far)
            : Entity(scene), window(window), sensitivity(sensitivity), normalSpeed(0.1f), sprintSpeed(0.2f), targetFov(0.0f),
            lastScrollY(0.0f), realYaw(0.0f), realPitch(0.0f), interpolatedYaw(0.0f), interpolatedPitch(0.0f) {
            addComponent<Nickel2::CameraComponent>(window, fov, near, far, NICKEL2_EULER_OVERRIDE);
        }

        ~FirstPersonCamera() {}

        void onCreate() override {}
        void onDestroy() override {}

        void onUpdate(float deltaTime) override {
            if (window->input->getKey(NICKEL2_KEY_LEFT_ALT)) {
                window->input->setCursorVisible(true);
                return;
            }

            if (!glfwGetWindowAttrib(window->getGLFWWindow(), GLFW_FOCUSED)) return;
            float speed = window->input->getKey(NICKEL2_KEY_LEFT_CONTROL) ? sprintSpeed : normalSpeed;
            Nickel2::Transform* transform = getComponent<Nickel2::TransformComponent>().transform;
            Nickel2::Camera* camera = getComponent<Nickel2::CameraComponent>().camera;

            if (window->input->getKey(NICKEL2_KEY_W)) {
                transform->translate(speed * glm::vec3(glm::cos(glm::radians(camera->yaw)), 0.0f, glm::sin(glm::radians(camera->yaw))) * window->deltaTime * 10.0f);
            }
            
            if (window->input->getKey(NICKEL2_KEY_S)) {
                transform->translate(speed * -glm::vec3(glm::cos(glm::radians(camera->yaw)), 0.0f, glm::sin(glm::radians(camera->yaw))) * window->deltaTime * 10.0f);
            }

            if (window->input->getKey(NICKEL2_KEY_A)) {
                transform->translate(speed * -glm::normalize(glm::cross(camera->front, camera->up)) * window->deltaTime * 10.0f);
            }

            if (window->input->getKey(NICKEL2_KEY_D)) {
                transform->translate(speed * glm::normalize(glm::cross(camera->front, camera->up)) * window->deltaTime * 10.0f);
            }

            if (window->input->getKey(NICKEL2_KEY_SPACE)) {
                transform->translate(speed * camera->up * window->deltaTime * 10.0f);
            }

            if (window->input->getKey(NICKEL2_KEY_LEFT_SHIFT)) {
                transform->translate(speed * -camera->up * window->deltaTime * 10.0f);
            }

            double x, y;
            window->input->setCursorVisible(false);
            window->input->getCursorPosition(&x, &y);

            int32_t width, height;
            window->getSize(&width, &height);

            float xOffset = sensitivity * (x - (int32_t) (width / 2)) / width;
            float yOffset = sensitivity * (y - (int32_t) (height / 2)) / height;

            camera->yaw = realYaw;
            camera->pitch = realPitch;

            camera->yaw += xOffset;
            camera->pitch -= yOffset;

            camera->pitch = glm::clamp(camera->pitch, -89.99f, 89.99f);

            realYaw = camera->yaw;
            realPitch = camera->pitch;

            interpolatedPitch = glm::lerp(interpolatedPitch, realPitch, 0.2f * 50.0f * window->deltaTime);
            interpolatedYaw = glm::lerp(interpolatedYaw, realYaw, 0.2f * 50.0f * window->deltaTime);
            camera->pitch = interpolatedPitch;
            camera->yaw = interpolatedYaw;

            float change = lastScrollY - Nickel2::input::scrollY;
            lastScrollY = Nickel2::input::scrollY;

            targetFov = glm::clamp((targetFov += change * 5.0f), 10.0f, 130.0f);
            camera->fov = glm::lerp(camera->fov, targetFov, 0.15f * 50.0f * window->deltaTime);
            sensitivity = camera->fov * 1.5f + 10.0f;

            window->input->setCursorPosition(width / 2, height / 2);
        }
};

int32_t main() {
    std::filesystem::path path = std::filesystem::current_path();
    system((".\\ext\\mono\\bin\\mcs.bat -target:library -out:" + path.generic_string() + "/scripts/Assemblies/Nickel2-ScriptCore.dll " + path.generic_string() + "/scripts/Core/*.cs").c_str());
    system((".\\ext\\mono\\bin\\mcs.bat -target:library -reference:" + path.generic_string() + "/scripts/Assemblies/Nickel2-ScriptCore.dll " +  "-out:" + path.generic_string() + "/scripts/Assemblies/Main.dll " + path.generic_string() + "/scripts/Main.cs").c_str());
    mono_set_assemblies_path(".\\ext\\mono\\lib\\mono\\4.5");

    MonoDomain* rootDomain = mono_jit_init("Nickel2JITRuntime");
    mono_thread_set_main(mono_thread_current());

    NICKEL2_ADD_INTERNAL_CALL(Input_IsKeyDown);

    MonoDomain* appDomain = mono_domain_create_appdomain((char*) "Nickel2ScriptRuntime", nullptr);
    mono_domain_set(appDomain, true);

    MonoAssembly* coreAssembly = loadMonoAssembly(".\\scripts\\Assemblies\\Nickel2-ScriptCore.dll");
    MonoImage* coreImage = mono_assembly_get_image(coreAssembly);
    MonoClass* entityClass = mono_class_from_name(coreImage, "Nickel2", "Entity");

    MonoAssembly* appAssembly = loadMonoAssembly(".\\scripts\\Assemblies\\Main.dll");
    MonoImage* appImage = mono_assembly_get_image(appAssembly);
    MonoClass* mainClass = mono_class_from_name(appImage, "Main", "Test");

    MonoObject* mainInstance = mono_object_new(appDomain, mainClass);
    mono_runtime_object_init(mainInstance);

    MonoMethod* constructorMethod = mono_class_get_method_from_name(entityClass, ".ctor", 1);
    MonoMethod* onCreateMethod = mono_class_get_method_from_name(mainClass, "OnCreate", 0);
    MonoMethod* onUpdateMethod = mono_class_get_method_from_name(mainClass, "OnUpdate", 1);

    uint64_t id = 22;
    void* params = &id;
    MonoObject* exception1 = nullptr;
    mono_runtime_invoke(constructorMethod, mainInstance, &params, &exception1);
    MonoObject* exception2 = nullptr;
    mono_runtime_invoke(onCreateMethod, mainInstance, nullptr, &exception2);

    Nickel2::Context context;
    Nickel2::Window window(1200, 600, ("Nickel2 v" + (std::string) NICKEL2_VERSION_STRING).c_str(), Nickel2::Color(0, 0, 0), false, false, true, true);
    Nickel2::Renderer renderer(&window, "res/hdr_textures/kloppenheim_07_puresky_4k.hdr");
    Nickel2::Scene scene;

    FirstPersonCamera camera(&scene, &window, 90.0f, 0.1f, 0.01f, 1000.0f);
    camera.addComponent<Nickel2::ListenerComponent>(camera.getComponent<Nickel2::CameraComponent>());
    scene.setPrimaryCamera(&camera);

    Nickel2::Entity entity(&scene);
    entity.addComponent<Nickel2::MeshComponent>("../Afterburner/res/models/missiles/aim-7-sparrow.gltf");
    // entity.addComponent<Nickel2::SourceComponent>(camera.getComponent<Nickel2::ListenerComponent>(), "res/music.wav", true);
    // entity.getComponent<Nickel2::SourceComponent>()->play();

    Nickel2::Entity entity2 = scene.createEntity("test");
    entity2.addComponent<Nickel2::MeshComponent>("../Afterburner/res/models/f18f/scene.gltf");
    entity2.getComponent<Nickel2::TransformComponent>()->setScale(glm::vec3(0.1f));

    Nickel2::Entity plagueMask = scene.createEntity("plagueMask");
    plagueMask.addComponent<Nickel2::MeshComponent>("res/plague_mask/scene.gltf");
    plagueMask.getComponent<Nickel2::TransformComponent>()->setScale(glm::vec3(0.1f));

    Nickel2::Entity zbrush = scene.createEntity("zbrush");
    zbrush.addComponent<Nickel2::MeshComponent>("res/zbrush/scene.gltf");

    auto& meshes = entity2.getComponent<Nickel2::MeshComponent>()->getSubMeshes();

    Nickel2::SubMesh* landingOff = meshes["0_f18f-landing-off"];
    meshes.erase("0_f18f-landing-off");

    Nickel2::SubMesh* canopy = meshes["0_f18f-canopy"];
    canopy->material.albedo = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
    canopy->material.metallic = 1.0f;
    canopy->material.roughness = 0.0f;

    Nickel2::SubMesh* hud = meshes["0_f18f-hud"];
    hud->material.albedo = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
    hud->material.metallic = 1.0f;
    hud->material.roughness = 0.0f;

    Nickel2::Scene* currentScene = &scene;
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    int32_t gizmoType = ImGuizmo::TRANSLATE;
    Nickel2::Entity* currentEntity = &entity;

    float yaw = 45.0f, pitch = 45.0f;

    while (!window.shouldClose()) {
        context.makeCurrent();
        context.pollEvents();
        window.update();

        currentScene->update(window.deltaTime);

        scene.directionalLightPosition = glm::normalize(glm::vec3(
            cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch))
        ));

        float turnSpeed = 10.0f;
        if (window.input->getKey(NICKEL2_KEY_LEFT)) yaw -= turnSpeed * window.deltaTime;
        if (window.input->getKey(NICKEL2_KEY_RIGHT)) yaw += turnSpeed * window.deltaTime;
        if (window.input->getKey(NICKEL2_KEY_UP)) pitch -= turnSpeed * window.deltaTime;
        if (window.input->getKey(NICKEL2_KEY_DOWN)) pitch += turnSpeed * window.deltaTime;

        void* params = &window.deltaTime;
        MonoObject* exception = nullptr;
        mono_runtime_invoke(onUpdateMethod, mainInstance, &params, &exception);

        if (window.input->getKey(NICKEL2_KEY_R))
            renderer.reloadShaders();

        renderer.imGuiNewFrame();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
        ImGui::Begin("Viewport", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
        ImGui::SetWindowPos(ImVec2(0, 0));
        int32_t width, height;
        window.getSize(&width, &height);
        ImGui::SetWindowSize(ImVec2(width, height));
        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        glm::vec2 viewportBounds[2];
        viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y, viewportBounds[1].x - viewportBounds[0].x, viewportBounds[1].y - viewportBounds[0].y);

        if (window.input->getKey(NICKEL2_KEY_1)) gizmoType = ImGuizmo::TRANSLATE;
        if (window.input->getKey(NICKEL2_KEY_2)) gizmoType = ImGuizmo::ROTATE;
        if (window.input->getKey(NICKEL2_KEY_3)) gizmoType = ImGuizmo::SCALE;

        if (window.input->getKey(NICKEL2_KEY_4)) currentEntity = &entity;
        if (window.input->getKey(NICKEL2_KEY_5)) currentEntity = &plagueMask;
        if (window.input->getKey(NICKEL2_KEY_6)) currentEntity = &zbrush;

        const glm::mat4& cameraProjection = camera.getComponent<Nickel2::CameraComponent>()->getProjection();
        glm::mat4 cameraView = camera.getComponent<Nickel2::CameraComponent>()->getView();
        glm::mat4 transform = currentEntity->getComponent<Nickel2::TransformComponent>()->getWorldMatrix();

        float snapValues[3] = { 0.1f, 1.0f, 0.1f };

        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
            (ImGuizmo::OPERATION) gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
            nullptr, window.input->getKey(NICKEL2_KEY_LEFT_SHIFT) ? snapValues : nullptr);

        if (ImGuizmo::IsUsing()) {
            glm::vec3 translation, _rotation, scale;
            decomposeTransform(transform, translation, _rotation, scale);

            glm::vec3 deltaRotation = _rotation - rotation;
            currentEntity->getComponent<Nickel2::TransformComponent>()->setTranslation(translation);
            rotation += deltaRotation;
            currentEntity->getComponent<Nickel2::TransformComponent>()->setRotation(glm::degrees(rotation));
            currentEntity->getComponent<Nickel2::TransformComponent>()->setScale(scale);
        }

        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        window.clear();
        camera.getComponent<Nickel2::CameraComponent>()->updateMatrices(renderer.backgroundShader);
        renderer.renderBackground();
        renderer.render(currentScene, true, true, false);
        window.swapBuffers();
    }

    meshes["0_f18f-landing-off"] = landingOff;

    camera.destroy();
    renderer.destroy();
    window.destroy();
    context.destroy();

    mono_domain_set(mono_get_root_domain(), false);
    mono_domain_unload(appDomain);
    mono_jit_cleanup(rootDomain);
    return 0;
}
#else
#include <Nickel2/nkpch.hpp>
#include <Nickel2/Nickel2.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <random>

enum OpenGLDeclType {
    GLDECLTYPE_FLOAT1 = 0, GLDECLTYPE_FLOAT2 = 1,
    GLDECLTYPE_FLOAT3 = 2, GLDECLTYPE_FLOAT4 = 3,
    GLDECLTYPE_GLCOLOR = 4
};

enum OpenGLDeclUsage {
    GLDECLUSAGE_POSITION = 0, GLDECLUSAGE_BLENDWEIGHT,
    GLDECLUSAGE_BLENDINDICES, GLDECLUSAGE_NORMAL,
    GLDECLUSAGE_PSIZE, GLDECLUSAGE_TEXCOORD,
    GLDECLUSAGE_TANGENT = GLDECLUSAGE_TEXCOORD + 8,
    GLDECLUSAGE_BINORMAL, GLDECLUSAGE_TESSFACTOR,
    GLDECLUSAGE_POSITIONT, GLDECLUSAGE_COLOR,
    GLDECLUSAGE_FOG, GLDECLUSAGE_DEPTH, GLDECLUSAGE_SAMPLE
};

enum OpenGLFormat {
    GLFMT_UNKNOWN = 0,
    GLFMT_R8, GLFMT_R8G8,
    GLFMT_R8G8B8, GLFMT_R8G8B8_sRGB,
    GLFMT_B8G8R8, GLFMT_B8G8R8_sRGB,
    GLFMT_A8R8G8B8, GLFMT_A8R8G8B8_sRGB,
    GLFMT_A8B8G8R8, GLFMT_A8B8G8R8_sRGB,
    GLFMT_R16_UINT, GLFMT_D24S8, GLFMT_D32F,
    GLFMT_DXT1, GLFMT_DXT1_sRGB,
    GLFMT_DXT5, GLFMT_DXT5_sRGB,
    GLFMT_R16F, GLFMT_G16R16F, GLFMT_A16B16G16R16F,
    GLFMT_R32F, GLFMT_G32R32F, GLFMT_A32B32G32R32F
};

enum OpenGLPrimitiveType {
    GLPT_TRIANGLELIST = GL_TRIANGLES
};

enum OpenGLLockFlags {
    GLLOCK_READONLY = GL_MAP_READ_BIT,
    GLLOCK_DISCARD = GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_WRITE_BIT
};

enum OpenGLMeshFlags {
    GLMESH_DYNAMIC = 1, GLMESH_32BIT = 2
};

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

struct OpenGLVertexElement {
    uint16_t stream, offset;
    uint8_t type, usage;
    uint8_t usageIndex;
};

struct OpenGLVertexDeclaration {
    OpenGLVertexElement* elements;
    uint32_t stride;
};

struct OpenGLAttributeRange {
    uint32_t primitiveType, attribID;
    uint32_t indexStart, indexCount;
    uint32_t vertexStart, vertexCount;
    bool enabled;
};

class TerrainQuadTree {
    public:
        struct Node {
            int32_t subnodes[4];
            glm::vec2 start;
            float length;
            int32_t lod;

            Node();

            bool isLeaf() const {
                return (subnodes[0] == -1 && subnodes[1] == -1 && subnodes[2] == -1 && subnodes[3] == -1);
            }
        };

    private:
        typedef std::vector<Node> NodeList;
        typedef std::function<void(const TerrainQuadTree::Node&)> NodeCallback;

        NodeList nodes;
        Node root;
        int32_t numLods;
        int32_t meshdim;
        float patchlength;
        float maxcoverage;
        float screenarea;

        float calculateCoverage(const Node& node, const glm::mat4& proj, const glm::vec3& eye) const;
        bool isVisible(const Node& node, const glm::mat4& matrix) const;
        void internalTraverse(const Node& node, NodeCallback callback) const;

        int32_t findLeaf(const glm::vec2& point) const;
        int32_t buildTree(Node& node, const glm::mat4& matrix, const glm::mat4& proj, const glm::vec3& eye);

    public:
        TerrainQuadTree();

        void findSubsetPattern(int32_t outindices[4], const Node& node);
        void initialize(const glm::vec2& start, float size, int32_t lodcount, int32_t meshsize, float patchsize, float maxgridcoverage, float screensize);
        void rebuild(const glm::mat4& matrix, const glm::mat4& proj, const glm::vec3& eye);
        void traverse(NodeCallback callback) const;
};

#define CHOOPY_SCALE_CORRECTION	1.35f

TerrainQuadTree::Node::Node() {
    subnodes[0] = -1;
    subnodes[1] = -1;
    subnodes[2] = -1;
    subnodes[3] = -1;
    lod = 0, length = 0;
}

TerrainQuadTree::TerrainQuadTree() {
    numLods = 0;
    meshdim = 0;
    patchlength = 0;
    maxcoverage = 0;
    screenarea = 0;
}

void TerrainQuadTree::initialize(const glm::vec2& start, float size, int32_t lodcount, int32_t meshsize, float patchsize, float maxgridcoverage, float screensize) {
    root.start	= start;
    root.length	= size;
    numLods = lodcount;
    meshdim = meshsize;
    patchlength = patchsize;
    maxcoverage = maxgridcoverage;
    screenarea = screensize;
}

void TerrainQuadTree::rebuild(const glm::mat4& matrix, const glm::mat4& proj, const glm::vec3& eye) {
    nodes.clear();
    buildTree(root, matrix, proj, eye);
}

int32_t TerrainQuadTree::buildTree(Node& node, const glm::mat4& matrix, const glm::mat4& proj, const glm::vec3& eye) {
    if (!isVisible(node, matrix))
        return -1;

    float coverage = calculateCoverage(node, proj, eye);
    bool visible = true;

    if (coverage > maxcoverage && node.length > patchlength) {
        Node subnodes[4];
        subnodes[0].start = node.start;
        subnodes[1].start = { node.start.x + 0.5f * node.length, node.start.y };
        subnodes[2].start = { node.start.x + 0.5f * node.length, node.start.y + 0.5f * node.length };
        subnodes[3].start = { node.start.x, node.start.y + 0.5f * node.length };
        subnodes[0].length = subnodes[1].length = subnodes[2].length = subnodes[3].length = 0.5f * node.length;
        node.subnodes[0] = buildTree(subnodes[0], matrix, proj, eye);
        node.subnodes[1] = buildTree(subnodes[1], matrix, proj, eye);
        node.subnodes[2] = buildTree(subnodes[2], matrix, proj, eye);
        node.subnodes[3] = buildTree(subnodes[3], matrix, proj, eye);
        visible = !node.isLeaf();
    }

    if (visible) {
        int32_t lod = 0;

        for (lod = 0; lod < numLods - 1; ++lod) {
            if (coverage > maxcoverage) break;
            else coverage *= 4.0f;
        } node.lod = glm::min(lod, numLods - 2);
    } else {
        return -1;
    }

    int32_t position = (int32_t) nodes.size();
    nodes.push_back(node);
    return position;
}

class AABox {
    public:
        glm::vec3 min, max;

        AABox() {
            min[0] = min[1] = min[2] = FLT_MAX;
            max[0] = max[1] = max[2] = -FLT_MAX;
        }

        ~AABox() {}

        void add(float x, float y, float z){
            if (max.x < x) max.x = x;
            if (max.y < y) max.y = y;
            if (max.z < z) max.z = z;
            if (min.x > x) min.x = x;
            if (min.y > y) min.y = y;
            if (min.z > z) min.z = z;
        }

        void getCenter(glm::vec3& out)  const {
            out[0] = (min[0] + max[0]) * 0.5f;
            out[1] = (min[1] + max[1]) * 0.5f;
            out[2] = (min[2] + max[2]) * 0.5f;
        }

        void getHalfSize(glm::vec3& out) const {
            out[0] = (max[0] - min[0]) * 0.5f;
            out[1] = (max[1] - min[1]) * 0.5f;
            out[2] = (max[2] - min[2]) * 0.5f;
        }
};

float planeDotCoord(const glm::vec4& plane, const glm::vec3& p) {
    return (plane.x * p.x + plane.y * p.y + plane.z * p.z + plane.w);
}

void PlaneNormalize(glm::vec4& out, const glm::vec4& p) {
    float il = 1.0f / glm::length((const glm::vec3&) p);
    out[0] = p[0] * il;
    out[1] = p[1] * il;
    out[2] = p[2] * il;
    out[3] = p[3] * il;
}

void frustumPlanes(glm::vec4 out[6], const glm::mat4& matrix) {
    out[0] = { matrix[0][0] + matrix[0][3], matrix[1][0] + matrix[1][3], matrix[2][0] + matrix[2][3], matrix[3][0] + matrix[3][3] };
    out[1] = { matrix[0][3] - matrix[0][0], matrix[1][3] - matrix[1][0], matrix[2][3] - matrix[2][0], matrix[3][3] - matrix[3][0] };
    out[2] = { matrix[0][3] - matrix[0][1], matrix[1][3] - matrix[1][1], matrix[2][3] - matrix[2][1], matrix[3][3] - matrix[3][1] };
    out[3] = { matrix[0][1] + matrix[0][3], matrix[1][1] + matrix[1][3], matrix[2][1] + matrix[2][3], matrix[3][1] + matrix[3][3] };
    out[4] = { matrix[0][3] + matrix[0][2], matrix[1][3] + matrix[1][2], matrix[2][3] + matrix[2][2], matrix[3][3] + matrix[3][2] };
    out[5] = { matrix[0][3] - matrix[0][2], matrix[1][3] - matrix[1][2], matrix[2][3] - matrix[2][2], matrix[3][3] - matrix[3][2] };

    PlaneNormalize(out[0], out[0]);
    PlaneNormalize(out[1], out[1]);
    PlaneNormalize(out[2], out[2]);
    PlaneNormalize(out[3], out[3]);
    PlaneNormalize(out[4], out[4]);
    PlaneNormalize(out[5], out[5]);
}

int32_t frustumIntersect(const glm::vec4 frustum[6], const AABox& box) {
    glm::vec3 center, halfsize;
    float dist, maxdist;
    int result = 2;

    box.getCenter(center);
    box.getHalfSize(halfsize);

    for (int j = 0; j < 6; ++j) {
        const glm::vec4& plane = frustum[j];
        dist = planeDotCoord(plane, center);
        maxdist = fabs(plane[0] * halfsize[0]) + fabs(plane[1] * halfsize[1]) + fabs(plane[2] * halfsize[2]);
        if (dist < -maxdist) return 0;
        else if (fabs(dist) < maxdist) result = 1;
    } return result;
}

bool TerrainQuadTree::isVisible(const Node& node, const glm::mat4& matrix) const {
    AABox box;
    glm::vec4 planes[6];
    float length = node.length;
    box.add(node.start.x - CHOOPY_SCALE_CORRECTION, -0.01f, node.start.y - CHOOPY_SCALE_CORRECTION);
    box.add(node.start.x + length + CHOOPY_SCALE_CORRECTION, 0.01f, node.start.y + length + CHOOPY_SCALE_CORRECTION);
    frustumPlanes(planes, matrix);
    return (frustumIntersect(planes, box) > 0);
}

int32_t TerrainQuadTree::findLeaf(const glm::vec2& point) const {
    int32_t index = -1;
    int32_t size = (int32_t) nodes.size();

    Node node = nodes[size - 1];

    while (!node.isLeaf()) {
        bool found = false;

        for (int32_t i = 0; i < 4; ++i) {
            if ((index = node.subnodes[i]) == -1)
                continue;

            Node subnode = nodes[index];

            if (point.x >= subnode.start.x && point.x <= subnode.start.x + subnode.length &&
                point.y >= subnode.start.y && point.y <= subnode.start.y + subnode.length) {
                node = subnode;
                found = true;
                break;
            }
        } if (!found) return -1;
    } return index;
}

void TerrainQuadTree::findSubsetPattern(int32_t outindices[4], const Node& node) {
    outindices[0] = 0;
    outindices[1] = 0;
    outindices[2] = 0;
    outindices[3] = 0;

    glm::vec2 point_left = { node.start.x - 0.5f * patchlength, node.start.y + 0.5f * node.length };
    glm::vec2 point_right = { node.start.x + node.length + 0.5f * patchlength, node.start.y + 0.5f * node.length };
    glm::vec2 point_bottom = { node.start.x + 0.5f * node.length, node.start.y + node.length + 0.5f * patchlength };
    glm::vec2 point_top = { node.start.x + 0.5f * node.length, node.start.y - 0.5f * patchlength };

    int32_t adjacency[4] = {
        findLeaf(point_left),
        findLeaf(point_right),
        findLeaf(point_bottom),
        findLeaf(point_top)
    };

    for (int32_t i = 0; i < 4; ++i) {
        if (adjacency[i] != -1 && nodes[adjacency[i]].length > node.length * 0.999f) {
            const Node& adj = nodes[adjacency[i]];
            float scale = adj.length / node.length * (meshdim >> node.lod) / (meshdim >> adj.lod);
        
            if (scale > 3.999f)
                outindices[i] = 2;

            else if (scale > 1.999f)
                outindices[i] = 1;
        }
    }
}

void TerrainQuadTree::traverse(NodeCallback callback) const {
    if (nodes.size() == 0)
        return;

    internalTraverse(nodes.back(), callback);
}

float TerrainQuadTree::calculateCoverage(const Node& node, const glm::mat4& proj, const glm::vec3& eye) const {
    const static glm::vec2 samples[16] = {
        { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 },
        { 0.5f, 0.333f }, { 0.25f, 0.667f },
        { 0.75f, 0.111f }, { 0.125f, 0.444f },
        { 0.625f, 0.778f }, { 0.375f, 0.222f },
        { 0.875f, 0.556f }, { 0.0625f, 0.889f },
        { 0.5625f, 0.037f }, { 0.3125f, 0.37f },
        { 0.8125f, 0.704f }, { 0.1875f, 0.148f },
    };

    glm::vec3 test;
    float length = node.length;
    float gridlength = length / meshdim;
    float worldarea = gridlength * gridlength;
    float maxprojarea = 0;

    for (int32_t i = 0; i < 16; ++i) {
        test.x = (node.start.x - CHOOPY_SCALE_CORRECTION) + (length + 2 * CHOOPY_SCALE_CORRECTION) * samples[i].x;
        test.y = 0;
        test.z = (node.start.y - CHOOPY_SCALE_CORRECTION) + (length + 2 * CHOOPY_SCALE_CORRECTION) * samples[i].y;

        float dist = glm::length(test - eye);
        float projarea = (worldarea * proj[0][0] * proj[1][1]) / (dist * dist);
        if (maxprojarea < projarea) maxprojarea = projarea;
    } return maxprojarea * screenarea * 0.25f;
}

void TerrainQuadTree::internalTraverse(const Node& node, NodeCallback callback) const {
    if (node.isLeaf()) {
        callback(node);
    } else {
        if (node.subnodes[0] != -1)
            internalTraverse(nodes[node.subnodes[0]], callback);

        if (node.subnodes[1] != -1)
            internalTraverse(nodes[node.subnodes[1]], callback);

        if (node.subnodes[2] != -1)
            internalTraverse(nodes[node.subnodes[2]], callback);

        if (node.subnodes[3] != -1)
            internalTraverse(nodes[node.subnodes[3]], callback);
    }
}

class OpenGLMesh {
    friend bool createMesh(uint32_t, uint32_t, uint32_t, OpenGLVertexElement*, OpenGLMesh**);

    struct LockedData {
        void* ptr;
        uint32_t flags;
    };

    private:
        OpenGLAttributeRange* subsettable;
        OpenGLVertexDeclaration vertexDecl;
        
        uint32_t options;
        uint32_t subsetCount;
        uint32_t vertexCount;
        uint32_t indexCount;
        uint32_t vertexBuffer;
        uint32_t indexBuffer;
        uint32_t vertexLayout;

        LockedData vertexLocked;
        LockedData indexLocked;

        OpenGLMesh();

        void destroy();
        void recreateVertexLayout();

    public:
        ~OpenGLMesh();

        void drawSubset(uint32_t subset);
        void setAttributeTable(const OpenGLAttributeRange* table, uint32_t size);
        bool lockVertexBuffer(uint32_t offset, uint32_t size, uint32_t flags, void** data);
        bool lockIndexBuffer(uint32_t offset, uint32_t size, uint32_t flags, void** data);
        void unlockVertexBuffer();
        void unlockIndexBuffer();

        uint32_t getNumSubsets() const {
            return subsetCount;
        }
};

bool createMesh(uint32_t vertexCount, uint32_t indexCount, uint32_t options, OpenGLVertexElement* decl, OpenGLMesh** mesh) {
    OpenGLMesh* newMesh = new OpenGLMesh();
    int32_t numdeclelems = 0;

    for (int32_t i = 0; i < 16; ++i) {
        ++numdeclelems;

        if (decl[i].stream == 0xff)
            break;
    }

    glGenBuffers(1, &newMesh->vertexBuffer);

    if (vertexCount >= 0xffff)
        options |= GLMESH_32BIT;

    newMesh->options = options;
    newMesh->subsetCount = 1;
    newMesh->vertexCount = vertexCount;
    newMesh->indexCount = indexCount;
    newMesh->subsettable = new OpenGLAttributeRange[1];
    newMesh->subsettable->attribID = 0;
    newMesh->subsettable->indexCount = indexCount;
    newMesh->subsettable->indexStart = 0;
    newMesh->subsettable->primitiveType = GLPT_TRIANGLELIST;
    newMesh->subsettable->vertexCount = (indexCount > 0 ? 0 : vertexCount);
    newMesh->subsettable->vertexStart = 0;
    newMesh->subsettable->enabled = GL_TRUE;
    newMesh->vertexDecl.elements = new OpenGLVertexElement[numdeclelems];
    memcpy(newMesh->vertexDecl.elements, decl, numdeclelems * sizeof(OpenGLVertexElement));
    newMesh->recreateVertexLayout();

    uint32_t usage = ((options & GLMESH_DYNAMIC) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    uint32_t istride = ((options & GLMESH_32BIT) ? 4 : 2);

    glBindBuffer(GL_ARRAY_BUFFER, newMesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * newMesh->vertexDecl.stride, 0, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (indexCount > 0) {
        glGenBuffers(1, &newMesh->indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * istride, 0, usage);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    (*mesh) = newMesh;
    return true;
}

#define GL_SAFE_DELETE_BUFFER(x) \
    if (x != 0) { \
        glDeleteBuffers(1, &x); \
        x = 0; }

OpenGLMesh::OpenGLMesh() {
    subsetCount = 0;
    vertexCount = 0;
    indexCount = 0;
    subsettable = nullptr;
    vertexBuffer = 0;
    indexBuffer = 0;
    vertexLayout= 0;
    options = 0;
    vertexDecl.elements = 0;
    vertexDecl.stride = 0;
    vertexLocked.ptr = 0;
    indexLocked.ptr = 0;
}

OpenGLMesh::~OpenGLMesh() {
    destroy();
}

void OpenGLMesh::destroy() {
    delete[] vertexDecl.elements;

    GL_SAFE_DELETE_BUFFER(vertexBuffer);
    GL_SAFE_DELETE_BUFFER(indexBuffer);

    if (vertexLayout != 0) {
        glDeleteVertexArrays(1, &vertexLayout);
        vertexLayout = 0;
    }

    delete[] subsettable;
    subsettable = nullptr;
    subsetCount = 0;
}

void OpenGLMesh::recreateVertexLayout() {
    if (vertexLayout != 0)
        glDeleteVertexArrays(1, &vertexLayout);

    glGenVertexArrays(1, &vertexLayout);
    vertexDecl.stride = 0;

    for (int32_t i = 0; i < 16; ++i) {
        OpenGLVertexElement& elem = vertexDecl.elements[i];

        if (elem.stream == 0xff)
            break;

        switch (elem.type) {
            case GLDECLTYPE_GLCOLOR:
            case GLDECLTYPE_FLOAT1: vertexDecl.stride += 4; break;
            case GLDECLTYPE_FLOAT2: vertexDecl.stride += 8; break;
            case GLDECLTYPE_FLOAT3: vertexDecl.stride += 12; break;
            case GLDECLTYPE_FLOAT4: vertexDecl.stride += 16; break;
            default: break;
        }
    }

    glBindVertexArray(vertexLayout); {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        for (int32_t i = 0; i < 16; ++i) {
            OpenGLVertexElement& elem = vertexDecl.elements[i];

            if (elem.stream == 0xff)
                break;

            glEnableVertexAttribArray(elem.usage);

            switch (elem.usage) {
            case GLDECLUSAGE_POSITION:
                glVertexAttribPointer(elem.usage, (elem.type == GLDECLTYPE_FLOAT4 ? 4 : 3), GL_FLOAT, GL_FALSE, vertexDecl.stride, (void*) (uint64_t) elem.offset);
                break;

            case GLDECLUSAGE_COLOR:
                glVertexAttribPointer(elem.usage, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexDecl.stride, (void*) (uint64_t) elem.offset);
                break;

            case GLDECLUSAGE_NORMAL:
                glVertexAttribPointer(elem.usage, (elem.type == GLDECLTYPE_FLOAT4 ? 4 : 3), GL_FLOAT, GL_FALSE, vertexDecl.stride, (void*) (uint64_t) elem.offset);
                break;

            case GLDECLUSAGE_TEXCOORD:
                // haaack...
                glVertexAttribPointer(elem.usage + elem.usageIndex, (elem.type + 1), GL_FLOAT, GL_FALSE, vertexDecl.stride, (void*) (uint64_t) elem.offset);
                break;

            case GLDECLUSAGE_TANGENT:
                glVertexAttribPointer(elem.usage, 3, GL_FLOAT, GL_FALSE, vertexDecl.stride, (void*) (uint64_t) elem.offset);
                break;

            case GLDECLUSAGE_BINORMAL:
                glVertexAttribPointer(elem.usage, 3, GL_FLOAT, GL_FALSE, vertexDecl.stride, (void*) (uint64_t) elem.offset);
                break;

            default:
                std::cout << "Unhandled layout element...\n";
                break;
            }
        }
    } glBindVertexArray(0);
}

bool OpenGLMesh::lockVertexBuffer(uint32_t offset, uint32_t size, uint32_t flags, void** data) {
    if (offset >= vertexCount * vertexDecl.stride) {
        (*data) = nullptr;
        return false;
    }

    if (size == 0)
        size = vertexCount * vertexDecl.stride - offset;

    if (flags == 0)
        flags = GL_MAP_READ_BIT|GL_MAP_WRITE_BIT;

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    vertexLocked.ptr = glMapBufferRange(GL_ARRAY_BUFFER, offset, size, flags);
    vertexLocked.flags = flags;

    if (!vertexLocked.ptr)
        return false;

    (*data) = vertexLocked.ptr;
    return true;
}

bool OpenGLMesh::lockIndexBuffer(uint32_t offset, uint32_t size, uint32_t flags, void** data) {
    uint32_t istride = ((options & GLMESH_32BIT) ? 4 : 2);

    if (offset >= indexCount * istride) {
        (*data) = nullptr;
        return false;
    }

    if (size == 0)
        size = indexCount * istride - offset;

    if (flags == 0)
        flags = GL_MAP_READ_BIT|GL_MAP_WRITE_BIT;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    indexLocked.ptr = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, size, flags);
    indexLocked.flags = flags;

    if (!indexLocked.ptr)
        return false;

    (*data) = indexLocked.ptr;
    return true;
}

void OpenGLMesh::drawSubset(uint32_t subset) {
    if (vertexLayout == 0 || vertexCount == 0)
        return;

    if (subsettable != nullptr && subset < subsetCount) {
        const OpenGLAttributeRange& attr = subsettable[subset];

        if (!attr.enabled)
            return;

        uint32_t itype = (options & GLMESH_32BIT) ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
        uint32_t start = attr.indexStart * ((options & GLMESH_32BIT) ? 4 : 2);

        glBindVertexArray(vertexLayout);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

        if (attr.indexCount == 0) {
            glDrawArrays(attr.primitiveType, attr.vertexStart, attr.vertexCount);
        } else {
            if (attr.vertexCount == 0)
                glDrawElements(attr.primitiveType, attr.indexCount, itype, (char*)0 + start);
            else
                glDrawRangeElements(attr.primitiveType, attr.vertexStart, attr.vertexStart + attr.vertexCount - 1, attr.indexCount, itype, (char*)0 + start);
        }
    }
}

void OpenGLMesh::unlockVertexBuffer() {
    if (vertexLocked.ptr != nullptr && vertexBuffer != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        vertexLocked.ptr = nullptr;
    }
}

void OpenGLMesh::unlockIndexBuffer() {
    if (indexLocked.ptr != nullptr && indexBuffer != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
        indexLocked.ptr = nullptr;
    }
}

void OpenGLMesh::setAttributeTable(const OpenGLAttributeRange* table, uint32_t size) {
    delete[] subsettable;
    subsettable = new OpenGLAttributeRange[size];
    memcpy(subsettable, table, size * sizeof(OpenGLAttributeRange));
    subsetCount = size;
}

#define DISP_MAP_SIZE 512
#define MESH_SIZE 256
#define GRAV_ACCELERATION 9.81f
#define PATCH_SIZE 20.0f
#define FURTHEST_COVER 8
#define MAX_COVERAGE 64.0f
#define WIND_DIRECTION { -0.4f, -0.9f }
#define WIND_SPEED 6.5f
#define AMPLITUDE_CONSTANT (0.45f * 1e-3f)

static const int32_t IndexCounts[] = {
    0, 0, 0, 0, 0, 0,
    961920, 3705084, 14500728
};

std::vector<glm::vec4> oceanColors = {
    { 0.0056f, 0.0194f, 0.0331f, 1.0f },
    { 0.1812f, 0.4678f, 0.5520f, 1.0f },
    { 0.0000f, 0.2307f, 0.3613f, 1.0f },
    { 0.2122f, 0.6105f, 1.0000f, 1.0f },
    { 0.0123f, 0.3613f, 0.6867f, 1.0f },
    { 0.0000f, 0.0999f, 0.4508f, 1.0f },
    { 0.0000f, 0.0331f, 0.1329f, 1.0f },
    { 0.0000f, 0.0103f, 0.0331f, 1.0f }
};

OpenGLMesh* oceanMesh = nullptr;

Nickel2::Shader* updateSpectrum;
Nickel2::Shader* fourierDFT;
Nickel2::Shader* fourierFFT;
Nickel2::Shader* createdisp;
Nickel2::Shader* gradientShader;
Nickel2::Shader* oceanShader;

uint32_t initial = 0;
uint32_t frequencies = 0;
uint32_t updated[2] = { 0 };
uint32_t tempdata = 0;
uint32_t displacement = 0;
uint32_t gradients = 0;
uint32_t perlintex = 0;

TerrainQuadTree tree;

uint32_t numLods = 0;
int32_t currentColor = 7;
bool useFFT = true;

void fourierTransform(uint32_t spectrum);
void generateLODLevels(OpenGLAttributeRange** subsettable, uint32_t* subsetCount, uint32_t* idata);
uint32_t generateBoundaryMesh(int32_t degLeft, int32_t degTop, int32_t degRight, int32_t degBottom, int32_t levelSize, uint32_t* idata);

static float phillips(const glm::vec2& k, const glm::vec2& w, float V, float A) {
    float L = (V * V) / GRAV_ACCELERATION;
    float l = L / 1000.0f;
    float kdotw = glm::dot(k, w);
    float k2 = glm::dot(k, k);
    float P_h = A * (expf(-1.0f / (k2 * L * L))) / (k2 * k2 * k2) * (kdotw * kdotw);
    if (kdotw < 0.0f) P_h *= 0.07f;
    return P_h * expf(-k2 * l * l);
}

static uint32_t calcSubsetIndex(int32_t level, int32_t dL, int32_t dR, int32_t dB, int32_t dT) {
    return 2 * (level * 3 * 3 * 3 * 3 + dL * 3 * 3 * 3 + dR * 3 * 3 + dB * 3 + dT);
}

enum DDS_Image_Type {
    DDSImageType2D = 0,
    DDSImageTypeCube,
    DDSImageTypeVolume
};

struct DDS_Image_Info {
    uint32_t Width;
    uint32_t Height;
    uint32_t Depth;
    uint32_t Format;
    uint32_t MipLevels;
    uint32_t DataSize;
    void* Data;
    DDS_Image_Type type;
};

struct DDS_PIXELFORMAT {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwFourCC;
    DWORD dwRGBBitCount;
    DWORD dwRBitMask;
    DWORD dwGBitMask;
    DWORD dwBBitMask;
    DWORD dwABitMask;
};

struct DDS_HEADER {
    DWORD dwSize;
    DWORD dwHeaderFlags;
    DWORD dwHeight;
    DWORD dwWidth;
    DWORD dwPitchOrLinearSize;
    DWORD dwDepth;
    DWORD dwMipMapCount;
    DWORD dwReserved1[11];

    DDS_PIXELFORMAT ddspf;

    DWORD dwCaps;
    DWORD dwCaps2;
    DWORD dwCaps3;
    DWORD dwCaps4;
    DWORD dwReserved2;
};

#define DDS_MAGIC 0x20534444
#define DDPF_FOURCC 0x00000004

#define FORMAT_R8G8B8 GLFMT_R8G8B8
#define FORMAT_B8G8R8 GLFMT_B8G8R8
#define FORMAT_A8R8G8B8 GLFMT_A8R8G8B8
#define FORMAT_DXT1 GLFMT_DXT1
#define FORMAT_DXT5 GLFMT_DXT5
#define FORMAT_G16R16F GLFMT_G16R16F
#define FORMAT_A16B16G16R16F GLFMT_A16B16G16R16F
#define FORMAT_G32R32F GLFMT_G32R32F

#define DDSCAPS_MIPMAP 0x400000

#define DDSCAPS2_CUBEMAP 0x200
#define DDSCAPS2_VOLUME 0x200000

#ifndef MAKEFOURCC
#	define MAKEFOURCC(ch0, ch1, ch2, ch3) \
        ((DWORD)(BYTE)(ch0)|((DWORD)(BYTE)(ch1) << 8)| \
        ((DWORD)(BYTE)(ch2) << 16)|((DWORD)(BYTE)(ch3) << 24))
#endif

const DDS_PIXELFORMAT DDSPF_DXT1 = {
    sizeof(DDS_PIXELFORMAT),
    DDPF_FOURCC,
    MAKEFOURCC('D','X','T','1'),
    0, 0, 0, 0, 0
};

const DDS_PIXELFORMAT DDSPF_DXT5 = {
    sizeof(DDS_PIXELFORMAT),
    DDPF_FOURCC,
    MAKEFOURCC('D','X','T','5'),
    0, 0, 0, 0, 0
};

uint32_t getImageSize(uint32_t width, uint32_t height, uint32_t bytes, uint32_t miplevels) {
    uint32_t w = width;
    uint32_t h = height;
    uint32_t bytesize = 0;

    for (uint32_t i = 0; i < miplevels; ++i) {
        bytesize += glm::max<uint32_t>(1, w) * glm::max<uint32_t>(1, h) * bytes;
        w = glm::max<uint32_t>(w / 2, 1);
        h = glm::max<uint32_t>(h / 2, 1);
    } return bytesize;
}

uint32_t getCompressedImageSize(uint32_t width, uint32_t height, uint32_t miplevels, uint32_t format) {
    uint32_t w = width;
    uint32_t h = height;
    uint32_t bytesize = 0;

    if (format == FORMAT_DXT1 || format == FORMAT_DXT5) {
        uint32_t mult = ((format == FORMAT_DXT5) ? 16 : 8);

        if (w != h) {
            for (uint32_t i = 0; i < miplevels; ++i) {
                bytesize += glm::max<uint32_t>(1, w / 4) * glm::max<uint32_t>(1, h / 4) * mult;
                w = glm::max<uint32_t>(w / 2, 1);
                h = glm::max<uint32_t>(h / 2, 1);
            }
        } else {
            bytesize = ((w / 4) * (h / 4) * mult);
            w = bytesize;

            for (uint32_t i = 1; i < miplevels; ++i) {
                w = glm::max(mult, w / 4);
                bytesize += w;
            }
        }
    } return bytesize;
}

uint32_t getCompressedImageSize(uint32_t width, uint32_t height, uint32_t depth, uint32_t miplevels, uint32_t format) {
    uint32_t w = width;
    uint32_t h = height;
    uint32_t d = depth;
    uint32_t bytesize = 0;

    if (format == FORMAT_DXT1 || format == FORMAT_DXT5) {
        uint32_t mult = ((format == FORMAT_DXT5) ? 16 : 8);

        for (uint32_t i = 0; i < miplevels; ++i) {
            bytesize += glm::max<uint32_t>(1, w / 4) * glm::max<uint32_t>(1, h / 4) * d * mult;

            w = glm::max<uint32_t>(w / 2, 1);
            h = glm::max<uint32_t>(h / 2, 1);
        }
    } return bytesize;
}

bool loadFromDDS(const char* file, DDS_Image_Info* outinfo) {
    DDS_HEADER header;
    FILE* infile = 0;
    DWORD magic;
    UINT bytesize = 0;

    if (!outinfo)
        return false;

    fopen_s(&infile, file, "rb");

    if (!infile)
        return false;

    fread(&magic, sizeof(DWORD), 1, infile);

    if (magic != DDS_MAGIC)
        goto _fail;

    fread(&header.dwSize, sizeof(DWORD), 1, infile);
    fread(&header.dwHeaderFlags, sizeof(DWORD), 1, infile);
    fread(&header.dwHeight, sizeof(DWORD), 1, infile);
    fread(&header.dwWidth, sizeof(DWORD), 1, infile);
    fread(&header.dwPitchOrLinearSize, sizeof(DWORD), 1, infile);
    fread(&header.dwDepth, sizeof(DWORD), 1, infile);
    fread(&header.dwMipMapCount, sizeof(DWORD), 1, infile);
    fread(&header.dwReserved1, sizeof(header.dwReserved1), 1, infile);
    fread(&header.ddspf.dwSize, sizeof(DWORD), 1, infile);
    fread(&header.ddspf.dwFlags, sizeof(DWORD), 1, infile);
    fread(&header.ddspf.dwFourCC, sizeof(DWORD), 1, infile);
    fread(&header.ddspf.dwRGBBitCount, sizeof(DWORD), 1, infile);
    fread(&header.ddspf.dwRBitMask, sizeof(DWORD), 1, infile);
    fread(&header.ddspf.dwGBitMask, sizeof(DWORD), 1, infile);
    fread(&header.ddspf.dwBBitMask, sizeof(DWORD), 1, infile);
    fread(&header.ddspf.dwABitMask, sizeof(DWORD), 1, infile);
    fread(&header.dwCaps, sizeof(DWORD), 1, infile);
    fread(&header.dwCaps2, sizeof(DWORD), 1, infile);
    fread(&header.dwCaps3, sizeof(DWORD), 1, infile);
    fread(&header.dwCaps4, sizeof(DWORD), 1, infile);
    fread(&header.dwReserved2, sizeof(DWORD), 1, infile);

    if (header.dwSize != sizeof(DDS_HEADER) || header.ddspf.dwSize != sizeof(DDS_PIXELFORMAT))
        goto _fail;

    outinfo->Width = header.dwWidth;
    outinfo->Height = header.dwHeight;
    outinfo->Depth = header.dwDepth;
    outinfo->Format = 0;
    outinfo->MipLevels = (header.dwMipMapCount == 0 ? 1 : header.dwMipMapCount);
    outinfo->Data = 0;
    outinfo->type = DDSImageType2D;

    if (header.ddspf.dwFlags & DDPF_FOURCC) {
        if (header.ddspf.dwFourCC == DDSPF_DXT1.dwFourCC) {
            outinfo->Format = FORMAT_DXT1;
        } else if (header.ddspf.dwFourCC == DDSPF_DXT5.dwFourCC) {
            outinfo->Format = FORMAT_DXT5;
        } else if (header.ddspf.dwFourCC == 0x70) {
            outinfo->Format = FORMAT_G16R16F;
            header.ddspf.dwRGBBitCount = 32;
        } else if (header.ddspf.dwFourCC == 0x71) {
            outinfo->Format = FORMAT_A16B16G16R16F;
            header.ddspf.dwRGBBitCount = 64;
        } else if (header.ddspf.dwFourCC == 0x73) {
            outinfo->Format = FORMAT_G32R32F;
            header.ddspf.dwRGBBitCount = 64;
        } else {
            goto _fail;
        }
    } else if (header.ddspf.dwRGBBitCount == 32) {
        outinfo->Format = FORMAT_A8R8G8B8;
    } else if (header.ddspf.dwRGBBitCount == 24) {
        if (header.ddspf.dwRBitMask & 0x00ff0000) {
            outinfo->Format = FORMAT_B8G8R8;
        } else {
            outinfo->Format = FORMAT_R8G8B8;
        }
    } else {
        goto _fail;
    }

    if (header.dwCaps2 & DDSCAPS2_VOLUME) {
        outinfo->type = DDSImageTypeVolume;

        if (outinfo->Format == FORMAT_DXT1 || outinfo->Format == FORMAT_DXT5) {
            bytesize = getCompressedImageSize(outinfo->Width, outinfo->Height, outinfo->Depth, outinfo->MipLevels, outinfo->Format);
            outinfo->Data = malloc(bytesize);
            outinfo->DataSize = bytesize;
            fread(outinfo->Data, 1, bytesize, infile);
        } else {
            bytesize = getImageSize(outinfo->Width, outinfo->Height, (header.ddspf.dwRGBBitCount / 8), outinfo->MipLevels) * outinfo->Depth;
            outinfo->Data = malloc(bytesize);
            fread((char*)outinfo->Data, 1, bytesize, infile);
            outinfo->DataSize = bytesize;
        }
    } else if (header.dwCaps2 & DDSCAPS2_CUBEMAP) {
        outinfo->type = DDSImageTypeCube;

        if (outinfo->Format == FORMAT_DXT1 || outinfo->Format == FORMAT_DXT5) {
            bytesize = getCompressedImageSize(outinfo->Width, outinfo->Height, outinfo->MipLevels, outinfo->Format) * 6;
            outinfo->Data = malloc(bytesize);
            outinfo->DataSize = bytesize;
            fread(outinfo->Data, 1, bytesize, infile);
        } else {
            bytesize = getImageSize(outinfo->Width, outinfo->Height, (header.ddspf.dwRGBBitCount / 8), outinfo->MipLevels) * 6;
            outinfo->Data = malloc(bytesize);
            fread((char*)outinfo->Data, 1, bytesize, infile);
            outinfo->DataSize = bytesize;
        }
    } else {
        if (outinfo->Format == FORMAT_DXT1 || outinfo->Format == FORMAT_DXT5) {
            bytesize = getCompressedImageSize(outinfo->Width, outinfo->Height, outinfo->MipLevels, outinfo->Format);
            outinfo->Data = malloc(bytesize);
            outinfo->DataSize = bytesize;
            fread(outinfo->Data, 1, bytesize, infile);
        } else {
            bytesize = getImageSize(outinfo->Width, outinfo->Height, (header.ddspf.dwRGBBitCount / 8), outinfo->MipLevels);
            outinfo->Data = malloc(bytesize);
            outinfo->DataSize = bytesize;
            fread(outinfo->Data, 1, bytesize, infile);
        }
    }

_fail:
    fclose(infile);
    return (outinfo->Data != 0);
}

uint32_t getCompressedLevelSize(uint32_t width, uint32_t height, uint32_t level, uint32_t format) {
    uint32_t w = width;
    uint32_t h = height;
    uint32_t bytesize = 0;

    if (format == FORMAT_DXT1 || format == FORMAT_DXT5) {
        uint32_t mult = ((format == FORMAT_DXT5) ? 16 : 8);

        if (w != h) {
            w = glm::max<uint32_t>(w / (1 << level), 1);
            h = glm::max<uint32_t>(h / (1 << level), 1);
            bytesize = glm::max<uint32_t>(1, w / 4) * glm::max<uint32_t>(1, h / 4) * mult;
        } else {
            bytesize = ((w / 4) * (h / 4) * mult);

            if (level > 0) {
                w = bytesize;

                for (uint32_t i = 0; i < level; ++i)
                    w = glm::max<uint32_t>(mult, w / 4);

                bytesize = w;
            }
        }
    } return bytesize;
}

#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT 0x8C4D
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT 0x8C4F

#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

#define GL_RGBA32F_ARB 0x8814
#define GL_RGBA16F_ARB 0x881A

int32_t mapFormatInternal[] = {
    0, GL_R8, GL_RG8, GL_RGB8, GL_SRGB8,
    GL_RGB8,  GL_SRGB8, GL_RGBA8,
    GL_SRGB8_ALPHA8, GL_RGBA8,
    GL_SRGB8_ALPHA8, GL_R16UI,
    GL_DEPTH24_STENCIL8, GL_DEPTH_COMPONENT32F,
    GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
    GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT,
    GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
    GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT,
    GL_R16F, GL_RG16F, GL_RGBA16F_ARB,
    GL_R32F, GL_RG32F, GL_RGBA32F_ARB
};

uint32_t mapFormatFormat[] = {
    0, GL_RED, GL_RG, GL_RGB, GL_RGB,
    GL_BGR, GL_BGR, GL_RGBA, GL_RGBA,
    GL_BGRA, GL_BGRA, GL_RED_INTEGER,
    GL_DEPTH_STENCIL, GL_DEPTH_COMPONENT,
    GL_RGBA, GL_RGBA, GL_RGBA, GL_RGBA,
    GL_RED, GL_RG, GL_RGBA, GL_RED, GL_RG, GL_RGBA
};

uint32_t mapFormatType[] = {
    0, GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT,
    GL_UNSIGNED_INT_24_8, GL_FLOAT,
    GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE, GL_UNSIGNED_BYTE,
    GL_HALF_FLOAT, GL_HALF_FLOAT, GL_HALF_FLOAT,
    GL_FLOAT, GL_FLOAT, GL_FLOAT
};

uint32_t nextPowTwo(uint32_t x) {
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return ++x;
}

uint32_t logTwoOfPowTwo(uint32_t x) {
    uint32_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

bool createTextureFromDDS(const char* file, bool srgb, uint32_t* out) {
    DDS_Image_Info info;
    uint32_t texid;

    if (!loadFromDDS(file, &info)) {
        std::cout << "Error: Could not load texture!\n";
        return false;
    }

    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (info.MipLevels > 1) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    int32_t pow2w = nextPowTwo(info.Width);
    int32_t pow2h = nextPowTwo(info.Height);
    int32_t mipsize;
    uint32_t format = info.Format;

    if (info.Format == GLFMT_DXT1 || info.Format == GLFMT_DXT5) {
        if (srgb) {
            if (info.Format == GLFMT_DXT1) format = GLFMT_DXT1_sRGB;
            else format = GLFMT_DXT5_sRGB;
        }

        int32_t width = info.Width;
        int32_t height = info.Height;
        int32_t offset = 0;

        for (uint32_t j = 0; j < info.MipLevels; ++j) {
            mipsize = getCompressedLevelSize(info.Width, info.Height, j, info.Format);

            glCompressedTexImage2D(GL_TEXTURE_2D, j, mapFormatInternal[format],
                width, height, 0, mipsize, (char*) info.Data + offset);

            offset += mipsize;
            width = (pow2w >> (j + 1));
            height = (pow2h >> (j + 1));
        }
    } else {
        uint32_t bytes = 4;

        if (info.Format == GLFMT_G32R32F) {
            bytes = 8;
        } else if (info.Format == GLFMT_G16R16F) {
            bytes = 4;
        } else if (info.Format == GLFMT_R8G8B8 || info.Format == GLFMT_B8G8R8) {
            if (srgb) {
                format = info.Format + 1;
            } bytes = 3;
        }

        mipsize = info.Width * info.Height * bytes;

        glTexImage2D(GL_TEXTURE_2D, 0, mapFormatInternal[format], info.Width, info.Height, 0,
            mapFormatFormat[format], mapFormatType[format], (char*) info.Data);

        if (info.MipLevels > 1)
            glGenerateMipmap(GL_TEXTURE_2D);
    }

    if (info.Data)
        free(info.Data);

    uint32_t err = glGetError();

    if (err != GL_NO_ERROR) {
        glDeleteTextures(1, &texid), texid = 0;
        std::cout << "Error: Could not create texture!\n";
    }

    *out = texid;
    return (texid != 0);
}

std::string insertDefines(std::string source, const char* defines) {
    size_t pos = source.find("#version");
    pos = source.find('\n', pos) + 1;
    source.insert(pos, defines);
    return source;
}

bool initScene(Nickel2::Window* window) {
    std::mt19937 gen;
    std::normal_distribution<> gaussian(0.0, 1.0);
    int32_t maxanisotropy = 1;

    int32_t screenwidth, screenheight;
    window->getSize(&screenwidth, &screenheight);

    glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);

    glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxanisotropy);
    maxanisotropy = glm::max(maxanisotropy, 2);

    glGenTextures(1, &initial);
    glGenTextures(1, &frequencies);

    glBindTexture(GL_TEXTURE_2D, initial);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG32F, DISP_MAP_SIZE + 1, DISP_MAP_SIZE + 1);

    glBindTexture(GL_TEXTURE_2D, frequencies);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, DISP_MAP_SIZE + 1, DISP_MAP_SIZE + 1);

    std::vector<float> h0data, wdata; {
        glm::vec2 wn = glm::normalize(glm::vec2(WIND_DIRECTION)), k;
        float V = WIND_SPEED;
        float A = AMPLITUDE_CONSTANT;

        for (int32_t m = 0; m <= DISP_MAP_SIZE; ++m) {
            k.y = (glm::two_pi<float>() * ((DISP_MAP_SIZE / 2) - m)) / PATCH_SIZE;

            for (int32_t n = 0; n <= DISP_MAP_SIZE; ++n) {
                k.x = (glm::two_pi<float>() * ((DISP_MAP_SIZE / 2) - n)) / PATCH_SIZE;
                float sqrtPh = 0;

                if (k.x != 0.0f || k.y != 0.0f) sqrtPh = sqrtf(phillips(k, wn, V, A));
                h0data.push_back((float) (sqrtPh * gaussian(gen) * glm::one_over_root_two<float>()));
                h0data.push_back((float) (sqrtPh * gaussian(gen) * glm::one_over_root_two<float>()));
                wdata.push_back(sqrtf(GRAV_ACCELERATION * glm::length(k)));
            }
        }
    }

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, DISP_MAP_SIZE + 1, DISP_MAP_SIZE + 1, GL_RED, GL_FLOAT, wdata.data());

    glBindTexture(GL_TEXTURE_2D, initial);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, DISP_MAP_SIZE + 1, DISP_MAP_SIZE + 1, GL_RG, GL_FLOAT, h0data.data());

    glGenTextures(2, updated);
    glBindTexture(GL_TEXTURE_2D, updated[0]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG32F, DISP_MAP_SIZE, DISP_MAP_SIZE);

    glBindTexture(GL_TEXTURE_2D, updated[1]);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG32F, DISP_MAP_SIZE, DISP_MAP_SIZE);

    glGenTextures(1, &tempdata);
    glBindTexture(GL_TEXTURE_2D, tempdata);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RG32F, DISP_MAP_SIZE, DISP_MAP_SIZE);

    glGenTextures(1, &displacement);
    glBindTexture(GL_TEXTURE_2D, displacement);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, DISP_MAP_SIZE, DISP_MAP_SIZE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenTextures(1, &gradients);
    glBindTexture(GL_TEXTURE_2D, gradients);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, DISP_MAP_SIZE, DISP_MAP_SIZE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxanisotropy / 2);

    glBindTexture(GL_TEXTURE_2D, 0);

    OpenGLVertexElement decl[] = {
        { 0, 0, GLDECLTYPE_FLOAT3, GLDECLUSAGE_POSITION, 0 },
        { 0xff, 0, 0, 0, 0 }
    };

    numLods = logTwoOfPowTwo(MESH_SIZE);

    if (!createMesh((MESH_SIZE + 1) * (MESH_SIZE + 1), IndexCounts[numLods], GLMESH_32BIT, decl, &oceanMesh))
        return false;

    OpenGLAttributeRange* subsettable = nullptr;
    glm::vec3* vdata = nullptr;
    uint32_t* idata = nullptr;
    uint32_t subsetCount = 0;

    oceanMesh->lockVertexBuffer(0, 0, GLLOCK_DISCARD, (void**) &vdata);
    oceanMesh->lockIndexBuffer(0, 0, GLLOCK_DISCARD, (void**) &idata); {
        for (int32_t z = 0; z <= MESH_SIZE; ++z) {
            for (int32_t x = 0; x <= MESH_SIZE; ++x) {
                int32_t index = z * (MESH_SIZE + 1) + x;
                vdata[index].x = (float) x;
                vdata[index].y = (float) z;
                vdata[index].z = 0.0f;
            }
        } generateLODLevels(&subsettable, &subsetCount, idata);
    }

    oceanMesh->unlockIndexBuffer();
    oceanMesh->unlockVertexBuffer();
    oceanMesh->setAttributeTable(subsettable, subsetCount);
    delete[] subsettable;
    char defines[128];

    sprintf_s(defines, "#define DISP_MAP_SIZE %d\n#define LOG2_DISP_MAP_SIZE %d\n#define TILE_SIZE_X2 %.4f\n#define INV_TILE_SIZE %.4f\n", \
        DISP_MAP_SIZE, logTwoOfPowTwo(DISP_MAP_SIZE), PATCH_SIZE * 2.0f / DISP_MAP_SIZE, DISP_MAP_SIZE / PATCH_SIZE);

    updateSpectrum = new Nickel2::Shader(insertDefines(Nickel2::readFile("shaders/ocean/updateSpectrum.comp"), defines));
    fourierDFT = new Nickel2::Shader(insertDefines(Nickel2::readFile("shaders/ocean/fourierDFT.comp"), defines));
    fourierFFT = new Nickel2::Shader(insertDefines(Nickel2::readFile("shaders/ocean/fourierFFT.comp"), defines));
    createdisp = new Nickel2::Shader(insertDefines(Nickel2::readFile("shaders/ocean/createDisplacement.comp"), defines));
    gradientShader = new Nickel2::Shader(insertDefines(Nickel2::readFile("shaders/ocean/createGradients.comp"), defines));
    oceanShader = new Nickel2::Shader(insertDefines(Nickel2::readFile("shaders/ocean/ocean.vert"), defines), insertDefines(Nickel2::readFile("shaders/ocean/ocean.frag"), defines));

    updateSpectrum->use();
    updateSpectrum->setUniform1i("tilde_h0", 0);
    updateSpectrum->setUniform1i("frequencies", 1);
    updateSpectrum->setUniform1i("tilde_h", 2);
    updateSpectrum->setUniform1i("tilde_D", 3);
    updateSpectrum->unuse();

    fourierDFT->use();
    fourierDFT->setUniform1i("readbuff", 0);
    fourierDFT->setUniform1i("writebuff", 1);
    fourierDFT->unuse();

    fourierFFT->use();
    fourierFFT->setUniform1i("readbuff", 0);
    fourierFFT->setUniform1i("writebuff", 1);
    fourierFFT->unuse();

    createdisp->use();
    createdisp->setUniform1i("heightmap", 0);
    createdisp->setUniform1i("choppyfield", 1);
    createdisp->setUniform1i("displacement", 2);
    createdisp->unuse();

    gradientShader->use();
    gradientShader->setUniform1i("displacement", 0);
    gradientShader->setUniform1i("gradients", 1);
    gradientShader->unuse();

    oceanShader->use();
    oceanShader->setUniform1i("displacement", 0);
    oceanShader->setUniform1i("perlin", 1);
    oceanShader->setUniform1i("envmap", 2);
    oceanShader->setUniform1i("gradients", 3);
    oceanShader->unuse();

    if (!createTextureFromDDS("res/perlin_noise.dds", false, &perlintex))
        return false;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxanisotropy / 2);

    float oceanExtent = PATCH_SIZE * (1 << FURTHEST_COVER);
    glm::vec2 oceanStart = { -0.5f * oceanExtent, -0.5f * oceanExtent };

    tree.initialize(oceanStart, oceanExtent, (int32_t) numLods, MESH_SIZE, PATCH_SIZE, MAX_COVERAGE, (float) (screenwidth * screenheight));
    return true;
}

#define CALC_INNER_INDEX(x, z) ((top + (z)) * (MESH_SIZE + 1) + left + (x))

void generateLODLevels(OpenGLAttributeRange** subsettable, uint32_t* subsetCount, uint32_t* idata) {
    assert(subsettable);
    assert(subsetCount);

    *subsetCount = (numLods - 2) * 3 * 3 * 3 * 3 * 2;
    *subsettable = new OpenGLAttributeRange[*subsetCount];

    int32_t currsubset = 0;
    uint32_t indexoffset = 0;
    uint32_t numWritten = 0;
    OpenGLAttributeRange* subset = 0;

    for (uint32_t level = 0; level < numLods - 2; ++level) {
        int32_t levelSize = MESH_SIZE >> level;
        int32_t mindegree = levelSize >> 3;

        for (int32_t degLeft = levelSize; degLeft > mindegree; degLeft >>= 1) {
            for (int32_t degRight = levelSize; degRight > mindegree; degRight >>= 1) {
                for (int32_t degBottom = levelSize; degBottom > mindegree; degBottom >>= 1) {
                    for (int32_t degTop = levelSize; degTop > mindegree; degTop >>= 1) {
                        int32_t right = ((degRight == levelSize) ? levelSize : levelSize - 1);
                        int32_t left = ((degLeft == levelSize) ? 0 : 1);
                        int32_t bottom = ((degBottom == levelSize) ? levelSize : levelSize - 1);
                        int32_t top = ((degTop == levelSize) ? 0 : 1);

                        int32_t width = right - left;
                        int32_t height = bottom - top;

                        numWritten = 0;

                        for (int32_t z = 0; z < height; ++z) {
                            if ((z & 1) == 1) {
                                idata[numWritten++] = CALC_INNER_INDEX(0, z);
                                idata[numWritten++] = CALC_INNER_INDEX(0, z + 1);

                                for (int32_t x = 0; x < width; ++x) {
                                    idata[numWritten++] = CALC_INNER_INDEX(x + 1, z);
                                    idata[numWritten++] = CALC_INNER_INDEX(x + 1, z + 1);
                                }

                                idata[numWritten++] = UINT32_MAX;
                            }
                            else {
                                idata[numWritten++] = CALC_INNER_INDEX(width, z + 1);
                                idata[numWritten++] = CALC_INNER_INDEX(width, z);

                                for (int32_t x = width - 1; x >= 0; --x) {
                                    idata[numWritten++] = CALC_INNER_INDEX(x, z + 1);
                                    idata[numWritten++] = CALC_INNER_INDEX(x, z);
                                }

                                idata[numWritten++] = UINT32_MAX;
                            }
                        }

                        subset = ((*subsettable) + currsubset);

                        subset->attribID = currsubset;
                        subset->enabled = (numWritten > 0);
                        subset->indexCount = numWritten;
                        subset->indexStart = indexoffset;
                        subset->primitiveType = GL_TRIANGLE_STRIP;
                        subset->vertexCount = 0;
                        subset->vertexStart = 0;

                        indexoffset += numWritten;
                        idata += numWritten;

                        ++currsubset;

                        numWritten = generateBoundaryMesh(degLeft, degTop, degRight, degBottom, levelSize, idata);

                        subset = ((*subsettable) + currsubset);

                        subset->attribID = currsubset;
                        subset->enabled = (numWritten > 0);
                        subset->indexCount = numWritten;
                        subset->indexStart = indexoffset;
                        subset->primitiveType = GL_TRIANGLES;
                        subset->vertexCount = 0;
                        subset->vertexStart = 0;

                        indexoffset += numWritten;
                        idata += numWritten;

                        ++currsubset;
                    }
                }
            }
        }
    }
}

#define CALC_BOUNDARY_INDEX(x, z) ((z) * (MESH_SIZE + 1) + (x))

uint32_t generateBoundaryMesh(int32_t degLeft, int32_t degTop, int32_t degRight, int32_t degBottom, int32_t levelSize, uint32_t* idata) {
    uint32_t numWritten = 0;

    if (degTop < levelSize) {
        int32_t t_step = levelSize / degTop;

        for (int32_t i = 0; i < levelSize; i += t_step) {
            idata[numWritten++] = CALC_BOUNDARY_INDEX(i, 0);
            idata[numWritten++] = CALC_BOUNDARY_INDEX(i + t_step / 2, 1);
            idata[numWritten++] = CALC_BOUNDARY_INDEX(i + t_step, 0);

            for (int32_t j = 0; j < t_step / 2; ++j) {
                if (i == 0 && j == 0 && degLeft < levelSize)
                    continue;

                idata[numWritten++] = CALC_BOUNDARY_INDEX(i, 0);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(i + j, 1);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(i + j + 1, 1);
            }

            for (int32_t j = t_step / 2; j < t_step; ++j) {
                if (i == levelSize - t_step && j == t_step - 1 && degRight < levelSize)
                    continue;

                idata[numWritten++] = CALC_BOUNDARY_INDEX(i + t_step, 0);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(i + j, 1);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(i + j + 1, 1);
            }
        }
    }

    if (degLeft < levelSize) {
        int32_t l_step = levelSize / degLeft;

        for (int32_t i = 0; i < levelSize; i += l_step) {
            idata[numWritten++] = CALC_BOUNDARY_INDEX(0, i);
            idata[numWritten++] = CALC_BOUNDARY_INDEX(0, i + l_step);
            idata[numWritten++] = CALC_BOUNDARY_INDEX(1, i + l_step / 2);

            for (int32_t j = 0; j < l_step / 2; ++j) {
                if (i == 0 && j == 0 && degTop < levelSize)
                    continue;

                idata[numWritten++] = CALC_BOUNDARY_INDEX(0, i);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(1, i + j + 1);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(1, i + j);
            }

            for (int32_t j = l_step / 2; j < l_step; ++j) {
                if (i == levelSize - l_step && j == l_step - 1 && degBottom < levelSize)
                    continue;

                idata[numWritten++] = CALC_BOUNDARY_INDEX(0, i + l_step);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(1, i + j + 1);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(1, i + j);
            }
        }
    }

    if (degRight < levelSize) {
        int32_t r_step = levelSize / degRight;

        for (int32_t i = 0; i < levelSize; i += r_step) {
            idata[numWritten++] = CALC_BOUNDARY_INDEX(levelSize, i);
            idata[numWritten++] = CALC_BOUNDARY_INDEX(levelSize - 1, i + r_step / 2);
            idata[numWritten++] = CALC_BOUNDARY_INDEX(levelSize, i + r_step);

            for (int32_t j = 0; j < r_step / 2; ++j) {
                if (i == 0 && j == 0 && degTop < levelSize)
                    continue;

                idata[numWritten++] = CALC_BOUNDARY_INDEX(levelSize, i);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(levelSize - 1, i + j);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(levelSize - 1, i + j + 1);
            }

            for (int32_t j = r_step / 2; j < r_step; ++j) {
                if (i == levelSize - r_step && j == r_step - 1 && degBottom < levelSize)
                    continue;

                idata[numWritten++] = CALC_BOUNDARY_INDEX(levelSize, i + r_step);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(levelSize - 1, i + j);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(levelSize - 1, i + j + 1);
            }
        }
    }

    if (degBottom < levelSize) {
        int32_t b_step = levelSize / degBottom;

        for (int32_t i = 0; i < levelSize; i += b_step) {
            idata[numWritten++] = CALC_BOUNDARY_INDEX(i, levelSize);
            idata[numWritten++] = CALC_BOUNDARY_INDEX(i + b_step, levelSize);
            idata[numWritten++] = CALC_BOUNDARY_INDEX(i + b_step / 2, levelSize - 1);

            for (int32_t j = 0; j < b_step / 2; ++j) {
                if (i == 0 && j == 0 && degLeft < levelSize)
                    continue;

                idata[numWritten++] = CALC_BOUNDARY_INDEX(i, levelSize);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(i + j + 1, levelSize - 1);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(i + j, levelSize - 1);
            }

            for (int32_t j = b_step / 2; j < b_step; ++j) {
                if (i == levelSize - b_step && j == b_step - 1 && degRight < levelSize)
                    continue;

                idata[numWritten++] = CALC_BOUNDARY_INDEX(i + b_step, levelSize);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(i + j + 1, levelSize - 1);
                idata[numWritten++] = CALC_BOUNDARY_INDEX(i + j, levelSize - 1);
            }
        }
    }

    return numWritten;
}

void terminateScene() {
    updateSpectrum->destroy();
    fourierDFT->destroy();
    fourierFFT->destroy();
    createdisp->destroy();
    gradientShader->destroy();
    oceanShader->destroy();

    delete updateSpectrum;
    delete fourierDFT;
    delete fourierFFT;
    delete createdisp;
    delete gradientShader;
    delete oceanShader;
    delete oceanMesh;

    glDeleteTextures(2, updated);
    glDeleteTextures(1, &displacement);
    glDeleteTextures(1, &gradients);
    glDeleteTextures(1, &initial);
    glDeleteTextures(1, &frequencies);
    glDeleteTextures(1, &tempdata);
    glDeleteTextures(1, &perlintex);
}

void fourierTransform(uint32_t spectrum) {
    Nickel2::Shader* shader = (useFFT ? fourierFFT : fourierDFT);

    glBindImageTexture(0, spectrum, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RG32F);
    glBindImageTexture(1, tempdata, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RG32F);

    shader->use(); {
        glDispatchCompute(DISP_MAP_SIZE, 1, 1);
    } shader->unuse();

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glBindImageTexture(0, tempdata, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RG32F);
    glBindImageTexture(1, spectrum, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RG32F);

    shader->use(); {
        glDispatchCompute(DISP_MAP_SIZE, 1, 1);
    } shader->unuse();
}

class FirstPersonCamera : public Nickel2::Entity {
    private:
        Nickel2::Window* window;

    public:
        float sensitivity, normalSpeed, sprintSpeed;
        float targetFov, lastScrollY, realYaw, realPitch, interpolatedYaw, interpolatedPitch;

        FirstPersonCamera(Nickel2::Scene* scene, Nickel2::Window* window, float fov, float sensitivity, float near, float far)
            : Entity(scene), window(window), sensitivity(sensitivity), normalSpeed(0.1f), sprintSpeed(0.2f), targetFov(fov),
            lastScrollY(0.0f), realYaw(0.0f), realPitch(0.0f), interpolatedYaw(0.0f), interpolatedPitch(0.0f) {
            addComponent<Nickel2::CameraComponent>(window, fov, near, far, NICKEL2_EULER_OVERRIDE);
        }

        ~FirstPersonCamera() {}

        void onCreate() override {}
        void onDestroy() override {}

        void onUpdate(float deltaTime) override {
            if (window->input->getKey(NICKEL2_KEY_LEFT_ALT)) {
                window->input->setCursorVisible(true);
                return;
            }

            if (!glfwGetWindowAttrib(window->getGLFWWindow(), GLFW_FOCUSED)) return;
            float speed = window->input->getKey(NICKEL2_KEY_LEFT_CONTROL) ? sprintSpeed : normalSpeed;
            Nickel2::Transform* transform = getComponent<Nickel2::TransformComponent>().transform;
            Nickel2::Camera* camera = getComponent<Nickel2::CameraComponent>().camera;

            if (window->input->getKey(NICKEL2_KEY_W)) {
                transform->translate(speed * glm::vec3(glm::cos(glm::radians(camera->yaw)), 0.0f, glm::sin(glm::radians(camera->yaw))) * window->deltaTime * 10.0f);
            }
            
            if (window->input->getKey(NICKEL2_KEY_S)) {
                transform->translate(speed * -glm::vec3(glm::cos(glm::radians(camera->yaw)), 0.0f, glm::sin(glm::radians(camera->yaw))) * window->deltaTime * 10.0f);
            }

            if (window->input->getKey(NICKEL2_KEY_A)) {
                transform->translate(speed * -glm::normalize(glm::cross(camera->front, camera->up)) * window->deltaTime * 10.0f);
            }

            if (window->input->getKey(NICKEL2_KEY_D)) {
                transform->translate(speed * glm::normalize(glm::cross(camera->front, camera->up)) * window->deltaTime * 10.0f);
            }

            if (window->input->getKey(NICKEL2_KEY_SPACE)) {
                transform->translate(speed * camera->up * window->deltaTime * 10.0f);
            }

            if (window->input->getKey(NICKEL2_KEY_LEFT_SHIFT)) {
                transform->translate(speed * -camera->up * window->deltaTime * 10.0f);
            }

            double x, y;
            window->input->setCursorVisible(false);
            window->input->getCursorPosition(&x, &y);

            int32_t width, height;
            window->getSize(&width, &height);

            float xOffset = sensitivity * (x - (int32_t) (width / 2)) / width;
            float yOffset = sensitivity * (y - (int32_t) (height / 2)) / height;

            camera->yaw = realYaw;
            camera->pitch = realPitch;

            camera->yaw += xOffset;
            camera->pitch -= yOffset;

            camera->pitch = glm::clamp(camera->pitch, -89.99f, 89.99f);

            realYaw = camera->yaw;
            realPitch = camera->pitch;

            interpolatedPitch = glm::lerp(interpolatedPitch, realPitch, 0.2f * 50.0f * window->deltaTime);
            interpolatedYaw = glm::lerp(interpolatedYaw, realYaw, 0.2f * 50.0f * window->deltaTime);
            camera->pitch = interpolatedPitch;
            camera->yaw = interpolatedYaw;

            float change = lastScrollY - Nickel2::input::scrollY;
            lastScrollY = Nickel2::input::scrollY;

            targetFov = glm::clamp((targetFov += change * 5.0f), 10.0f, 80.0f);
            camera->fov = glm::lerp(camera->fov, targetFov, 0.15f * 50.0f * window->deltaTime);
            sensitivity = camera->fov * 1.5f + 10.0f;

            window->input->setCursorPosition(width / 2, height / 2);
        }
};

void render(Nickel2::Window* window, Nickel2::Renderer* renderer, Nickel2::Camera* camera) {
    static float time = 0.0f;
    glm::mat4 world = glm::mat4(1.0f);

    int32_t screenwidth, screenheight;
    window->getSize(&screenwidth, &screenheight);

    glm::vec3 eye = camera->transform->getTranslation();
    glm::mat4 view = camera->getView();
    glm::mat4 proj = camera->getProjection();
    glm::mat4 matrix = proj * view;

    updateSpectrum->use();
    updateSpectrum->setUniform1f("time", time);
    updateSpectrum->unuse();

    glBindImageTexture(0, initial, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RG32F);
    glBindImageTexture(1, frequencies, 0, GL_TRUE, 0, GL_READ_ONLY, GL_R32F);

    glBindImageTexture(2, updated[0], 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RG32F);
    glBindImageTexture(3, updated[1], 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RG32F);

    updateSpectrum->use(); {
        glDispatchCompute(DISP_MAP_SIZE / 16, DISP_MAP_SIZE / 16, 1);
    } updateSpectrum->unuse();

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    fourierTransform(updated[0]);
    fourierTransform(updated[1]);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glBindImageTexture(0, updated[0], 0, GL_TRUE, 0, GL_READ_ONLY, GL_RG32F);
    glBindImageTexture(1, updated[1], 0, GL_TRUE, 0, GL_READ_ONLY, GL_RG32F);
    glBindImageTexture(2, displacement, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    createdisp->use(); {
        glDispatchCompute(DISP_MAP_SIZE / 16, DISP_MAP_SIZE / 16, 1);
    } createdisp->unuse();

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glBindImageTexture(0, displacement, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(1, gradients, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA16F);

    gradientShader->use(); {
        glDispatchCompute(DISP_MAP_SIZE / 16, DISP_MAP_SIZE / 16, 1);
    } gradientShader->unuse();

    glMemoryBarrier(GL_TEXTURE_UPDATE_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);

    glBindTexture(GL_TEXTURE_2D, gradients);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glDisable(GL_BLEND);
    
    tree.rebuild(matrix, proj, eye);

    glm::mat4 flipYZ = { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 };
    glm::mat4 localtraf = glm::mat4(1.0f);
    glm::vec2 w = WIND_DIRECTION;
    int32_t pattern[4];

    glm::vec4 uvparams = glm::vec4(1.0f / PATCH_SIZE, 0.5f / DISP_MAP_SIZE, 0.0f, 0.0f);
    glm::vec2 perlinoffset = glm::vec2(-w.x * time * 0.06f, -w.y * time * 0.06f);

    oceanShader->use();
    oceanShader->setUniformMatrix4fv("matViewProj", (float*) glm::value_ptr(matrix));
    oceanShader->setUniform2fv("perlinOffset", glm::value_ptr(perlinoffset));
    oceanShader->setUniform3fv("eyePos", glm::value_ptr(eye));
    oceanShader->setUniform3fv("oceanColor", glm::value_ptr(oceanColors[currentColor]));
    oceanShader->unuse();

    oceanShader->use(); {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, displacement);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, perlintex);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, renderer->getEnvironmentCubeMap());

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, gradients);

        tree.traverse([&](const TerrainQuadTree::Node& node) {
            float levelSize = (float) (MESH_SIZE >> node.lod);
            float scale = node.length / levelSize;

            localtraf[0][0] = scale;
            localtraf[1][1] = scale;
            localtraf[2][2] = 0.0f;

            world = glm::mat4(1.0f);

            world[3][0] = node.start[0];
            world[3][1] = 0.0f;
            world[3][2] = node.start[1];

            world *= flipYZ;

            uvparams.z = node.start[0] / PATCH_SIZE;
            uvparams.w = node.start[1] / PATCH_SIZE;

            oceanShader->setUniformMatrix4fv("matLocal", glm::value_ptr(localtraf));
            oceanShader->setUniformMatrix4fv("matWorld", glm::value_ptr(world));
            oceanShader->setUniform4fv("uvParams", glm::value_ptr(uvparams));

            tree.findSubsetPattern(pattern, node);
            uint32_t subset = calcSubsetIndex(node.lod, pattern[0], pattern[1], pattern[2], pattern[3]);

            if (subset < oceanMesh->getNumSubsets() - 1) {
                oceanMesh->drawSubset(subset);
                oceanMesh->drawSubset(subset + 1);
            }
        });

        glActiveTexture(GL_TEXTURE0);
    } oceanShader->unuse();

    time += window->deltaTime;

    glDisable(GL_FRAMEBUFFER_SRGB);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    uint32_t err = glGetError();

    if (err != GL_NO_ERROR)
        std::cout << "Error\n";
}

int32_t main(int32_t argc, char* argv[]) {
    Nickel2::Context context;
    Nickel2::Window window(1200, 600, "Nickel2", Nickel2::Color(0, 0, 0), false, false, true, true);
    Nickel2::Renderer renderer(&window, "res/hdr_textures/evening_road_01_puresky_4k.hdr");
    Nickel2::Scene scene;

    FirstPersonCamera camera(&scene, &window, 0.0f, 0.1f, 0.1f, 1000.0f);
    camera.addComponent<Nickel2::ListenerComponent>(camera.getComponent<Nickel2::CameraComponent>());
    scene.setPrimaryCamera(&camera);

    initScene(&window);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    Nickel2::Scene* currentScene = &scene;

    while (!window.shouldClose()) {
        context.makeCurrent();
        context.pollEvents();
        window.update();

        currentScene->update(window.deltaTime);

        if (window.input->getKey(NICKEL2_KEY_R))
            renderer.reloadShaders();

        if (window.input->getKey(NICKEL2_KEY_Q))
            renderer.loadHDRTexture("res/hdr_textures/evening_road_01_puresky_4k.hdr");
        
        if (window.input->getKey(NICKEL2_KEY_E))
            renderer.loadHDRTexture("res/hdr_textures/kloppenheim_07_puresky_4k.hdr");

        renderer.imGuiNewFrame();

        ImGui::Begin("Ocean Settings");

        if (ImGui::TreeNode("Ocean Colors")) {
            for (uint32_t i = 0; i < oceanColors.size(); i++) {
                ImGui::ColorEdit3(std::to_string(i).c_str(), glm::value_ptr(oceanColors[i]));
            } ImGui::TreePop();
        }

        ImGui::SliderInt("Color", &currentColor, 0, oceanColors.size() - 1);
        ImGui::Checkbox("Use FFT", &useFFT);
        ImGui::Image((void*) (intptr_t) displacement, ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::Image((void*) (intptr_t) gradients, ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        window.clear();
        camera.getComponent<Nickel2::CameraComponent>()->updateMatrices(renderer.backgroundShader);
        renderer.renderBackground();
        render(&window, &renderer, scene.getPrimaryCamera());
        renderer.render(currentScene, true, true, false);
        window.swapBuffers();
    }

    terminateScene();

    camera.destroy();
    renderer.destroy();
    window.destroy();
    context.destroy();
    return 0;
}
#endif