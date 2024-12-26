#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>

#include <al/al.h>
#include <al/alc.h>
#include <al/alut.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <glfw/glfw3native.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <imguizmo/ImGuizmo.h>

#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <entt/entt.hpp>

#ifndef JPH_ENABLE_ASSERTS
#define JPH_ENABLE_ASSERTS
#endif

#include <Jolt/Jolt.h>

#include "Audio/AudioAPI.hpp"
#include "Audio/AudioListener.hpp"
#include "Audio/AudioSource.hpp"
#include "Audio/AudioSystem.hpp"

#include "Core/Base.hpp"
#include "Core/Version.hpp"
#include "Core/Platform.hpp"
#include "Core/Logger.hpp"
#include "Core/Utils.hpp"
#include "Core/Color.hpp"
#include "Core/Input.hpp"
#include "Core/Window.hpp"
#include "Core/Application.hpp"
#include "Core/Layer.hpp"
#include "Core/LayerStack.hpp"
#include "Core/KeyCodes.hpp"
#include "Core/Entry.hpp"

#include "Event/Event.hpp"
#include "Event/KeyEvent.hpp"
#include "Event/MouseEvent.hpp"
#include "Event/WindowEvent.hpp"

#include "Math/Math.hpp"

#include "Physics/ColliderMaterial.hpp"
#include "Physics/MeshCookingFactory.hpp"
#include "Physics/PhysicsAPI.hpp"
#include "Physics/PhysicsBody.hpp"
#include "Physics/PhysicsLayer.hpp"
#include "Physics/PhysicsScene.hpp"
#include "Physics/PhysicsSettings.hpp"
#include "Physics/PhysicsShapes.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Physics/PhysicsTypes.hpp"

#include "Renderer/Buffer.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/FrameBuffer.hpp"
#include "Renderer/ImGuiLayer.hpp"
#include "Renderer/Material.hpp"
#include "Renderer/Mesh.hpp"
#include "Renderer/RenderCommand.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/RendererAPI.hpp"
#include "Renderer/RendererContext.hpp"
#include "Renderer/SceneRenderer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/Submesh.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/UniformBuffer.hpp"
#include "Renderer/Vertex.hpp"
#include "Renderer/VertexArray.hpp"

#include "Scene/Component.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Scene.hpp"

#ifdef NK_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>
#endif