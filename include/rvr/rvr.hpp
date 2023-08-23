#pragma once

#ifdef __MINGW32__
#include <openvr/openvr_mingw.hpp>
#else
#include <openvr/openvr.h>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <windows.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#ifdef RVR_EXTERN
extern "C" {
#endif

namespace rvr {
    extern vr::IVRSystem* hmd;
    extern vr::TrackedDevicePose_t trackedDevicePose[vr::k_unMaxTrackedDeviceCount];
    extern glm::mat4 devicePoseMatrix[vr::k_unMaxTrackedDeviceCount];
    extern char deviceClassChar[vr::k_unMaxTrackedDeviceCount];
    extern std::string poseClasses;
    extern unsigned int validPoseCount;
    extern glm::mat4 hmdPose;
    extern glm::mat4 leftProjectionMatrix;
    extern glm::mat4 rightProjectionMatrix;
    extern glm::mat4 leftEyePosMatrix;
    extern glm::mat4 rightEyePosMatrix;
    extern std::string actionManfiestPath;
    extern std::string driver;
    extern std::string display;

    struct FramebufferDesc {
        unsigned int depthBuffer;
        unsigned int renderTexture;
        unsigned int renderFramebuffer;
        unsigned int resolveTexture;
        unsigned int resolveFramebuffer;
    };

    extern FramebufferDesc leftEyeDesc;
    extern FramebufferDesc rightEyeDesc;

    extern unsigned int renderModelProgram, renderModelMatrixLocation;

    class RVRCGLRenderModel {
        private:
            std::string modelName;
            vr::RenderModel_t* renderModel;
            vr::RenderModel_TextureMap_t* textureMap;
            unsigned int vao, vbo, ibo, tex;

        public:
            bool init(vr::RenderModel_t* model, vr::RenderModel_TextureMap_t* texture);
            bool render();
            bool destroy();

            RVRCGLRenderModel(const std::string& renderModelName);
            ~RVRCGLRenderModel();

            const std::string& getName() const;
            vr::RenderModel_t* getRenderModel();
            vr::RenderModel_TextureMap_t* getTextureMap();
    };

    struct RVRControllerInfo {
        vr::VRInputValueHandle_t source = vr::k_ulInvalidInputValueHandle;
        vr::VRActionHandle_t actionPose = vr::k_ulInvalidActionHandle;
        vr::VRActionHandle_t actionHaptic = vr::k_ulInvalidActionHandle;
        vr::VRActionHandle_t actionJoystickClick = vr::k_ulInvalidActionHandle;
        vr::VRActionHandle_t actionJoystickTouch = vr::k_ulInvalidActionHandle;
        vr::VRActionHandle_t actionJoystickPosition = vr::k_ulInvalidActionHandle;
        vr::VRActionHandle_t actionTriggerClick = vr::k_ulInvalidActionHandle;
        vr::VRActionHandle_t actionTriggerTouch = vr::k_ulInvalidActionHandle;
        vr::VRActionHandle_t actionTriggerPull = vr::k_ulInvalidActionHandle;
        vr::VRActionHandle_t actionGripClick = vr::k_ulInvalidActionHandle;
        vr::VRActionHandle_t actionGripTouch = vr::k_ulInvalidActionHandle;
        vr::VRActionHandle_t actionGripPull = vr::k_ulInvalidActionHandle;
        vr::VRActionHandle_t actionButtonOneClick = vr::k_ulInvalidActionHandle;
        vr::VRActionHandle_t actionButtonOneTouch = vr::k_ulInvalidActionHandle;
        vr::VRActionHandle_t actionButtonTwoClick = vr::k_ulInvalidActionHandle;
        vr::VRActionHandle_t actionButtonTwoTouch = vr::k_ulInvalidActionHandle;
        glm::mat4 poseMatrix;
        RVRCGLRenderModel* renderModel = nullptr;
        std::string renderModelName;
        bool showController = true;
    };

    extern RVRControllerInfo RVRControllers[2];
    extern std::vector <RVRCGLRenderModel*> renderModels;
    extern vr::VRActionSetHandle_t actionSetRVR;

    typedef void (*RVRTrackedDeviceDeactivateCallback_T)(unsigned int);
    typedef void (*RVRTrackedDeviceUpdateCallback_T)(unsigned int);

    extern RVRTrackedDeviceDeactivateCallback_T* RVRTrackedDeviceDeactivateCallback;
    extern RVRTrackedDeviceUpdateCallback_T* RVRTrackedDeviceUpdateCallback;

    enum RVREye {
        RVREyeLeft = 0, RVREyeRight = 1
    };

    extern RVREye currentEye;

    enum RVRController {
        RVRControllerLeft = 0, RVRControllerRight = 1
    };

    bool RVRCreateFrameBuffer(int width, int height, FramebufferDesc& framebufferDesc);
    RVRCGLRenderModel* RVRFindOrLoadRenderModel(const char* renderModelName);
    std::string RVRGetTrackedDeviceString(vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError* peError = NULL);
    void RVRGetRecommendedRenderTargetSize(unsigned int* width, unsigned int* height);
    bool RVRIsReady();
    bool RVRSetupStereoRenderTargets();
    bool RVRBeginRendering(RVREye eye);
    bool RVREndRendering();
    bool RVRSubmitFramebufferDescriptorsToCompositor();
    bool RVRDeleteFramebufferDescriptors();
    unsigned int RVRCreateShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
    bool RVRInitControllers();
    RVRControllerInfo& RVRGetController(RVRController controller);
    void RVRSetControllerShowState(RVRController controller, bool state);
    bool RVRGetControllerShowState(RVRController controller);
    glm::mat4 RVRGetCurrentViewProjectionMatrix(RVREye eye);
    float* RVRGetCurrentViewProjectionMatrixArray(RVREye eye);
    glm::mat4 RVRGetCurrentViewProjectionNoPoseMatrix(RVREye eye);
    float* RVRGetCurrentViewProjectionNoPoseMatrixArray(RVREye eye);
    glm::vec3 RVRGetHmdDirection();
    bool RVRRenderControllers();
    void RVRSetActionManifestPath(const char* path);
    bool RVRInit();
    bool RVRGetDigitalActionRisingEdge(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr);
    bool RVRGetDigitalActionFallingEdge(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr);
    bool RVRGetDigitalActionState(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr);
    bool RVRGetControllerTriggerClickState(RVRController controller);
    bool RVRGetControllerTriggerClickRisingEdge(RVRController controller);
    bool RVRGetControllerTriggerClickFallingEdge(RVRController controller);
    bool RVRGetControllerGripClickState(RVRController controller);
    bool RVRGetControllerGripClickRisingEdge(RVRController controller);
    bool RVRGetControllerGripClickFallingEdge(RVRController controller);
    bool RVRGetControllerJoystickClickState(RVRController controller);
    bool RVRGetControllerJoystickClickRisingEdge(RVRController controller);
    bool RVRGetControllerJoystickClickFallingEdge(RVRController controller);
    bool RVRGetControllerTriggerTouchState(RVRController controller);
    bool RVRGetControllerTriggerTouchRisingEdge(RVRController controller);
    bool RVRGetControllerTriggerTouchFallingEdge(RVRController controller);
    bool RVRGetControllerGripTouchState(RVRController controller);
    bool RVRGetControllerGripTouchRisingEdge(RVRController controller);
    bool RVRGetControllerGripTouchFallingEdge(RVRController controller);
    bool RVRGetControllerJoystickTouchState(RVRController controller);
    bool RVRGetControllerJoystickTouchRisingEdge(RVRController controller);
    bool RVRGetControllerJoystickTouchFallingEdge(RVRController controller);
    bool RVRGetControllerButtonOneClickState(RVRController controller);
    bool RVRGetControllerButtonOneClickRisingEdge(RVRController controller);
    bool RVRGetControllerButtonOneClickFallingEdge(RVRController controller);
    bool RVRGetControllerButtonOneTouchState(RVRController controller);
    bool RVRGetControllerButtonOneTouchRisingEdge(RVRController controller);
    bool RVRGetControllerButtonOneTouchFallingEdge(RVRController controller);
    bool RVRGetControllerButtonTwoClickState(RVRController controller);
    bool RVRGetControllerButtonTwoClickRisingEdge(RVRController controller);
    bool RVRGetControllerButtonTwoClickFallingEdge(RVRController controller);
    bool RVRGetControllerButtonTwoTouchState(RVRController controller);
    bool RVRGetControllerButtonTwoTouchRisingEdge(RVRController controller);
    bool RVRGetControllerButtonTwoTouchFallingEdge(RVRController controller);
    glm::vec2 RVRGetAnalogActionData(vr::VRActionHandle_t action);
    float RVRGetControllerTriggerPull(RVRController controller);
    float RVRGetControllerGripPull(RVRController controller);
    glm::vec2 RVRGetControllerJoystickPosition(RVRController controller);
    void RVRTriggerHapticVibration(RVRController controller, float duration, float frequency, float amplitude);
    bool RVRShutdown();
    bool RVRIsInputAvailable();
    glm::mat4 RVRGetProjectionMatrix(RVREye eye, float nearClip, float farClip);
    float* RVRGetProjectionMatrixArray(RVREye eye, float nearClip, float farClip);
    glm::mat4 RVRGetEyePoseMatrix(RVREye eye);
    float* RVRGetEyePoseMatrixArray(RVREye eye);

    void RVRSetTrackedDeviceDeactivateCallback(RVRTrackedDeviceDeactivateCallback_T* func);
    void RVRSetTrackedDeviceUpdateCallback(RVRTrackedDeviceUpdateCallback_T* func);
    glm::mat4 RVRConvertOpenVRMatrixToGLMMatrix(const vr::HmdMatrix34_t& mat);
    float* RVRGetControllerPoseMatrixArray(RVRController controller);
    glm::mat4 RVRGetControllerPoseMatrix(RVRController controller);
    glm::vec3 RVRGetControllerPosition(RVRController controller);
    float* RVRGetHmdPoseMatrixArray();
    glm::mat4 RVRGetHmdPoseMatrix();
    glm::vec3 RVRGetHmdPosition();
    unsigned int RVRGetEyeResolveTexture(RVREye eye);
    void RVRCheckControllers();
    void RVRPollEvents();
    vr::ETrackedDeviceClass RVRGetTrackedDeviceClass(int number);
    void RVRCompositorWaitGetPoses(vr::TrackedDevicePose_t* renderPoseArray, unsigned int renderPoseArrayCount, vr::TrackedDevicePose_t* poseArray, unsigned int poseArrayCount);
    bool RVRInitEyes(float nearClip, float farClip);
    bool RVRInitCompositor();
    void RVRUpdateHMDPoseMatrix();
}

#ifdef RVR_EXTERN
}
#endif