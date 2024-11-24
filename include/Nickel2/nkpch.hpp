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

#include <Nickel2/Core/Platform.hpp>

#ifdef NK_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>
#endif

#include <Nickel2/Core/Base.hpp>
#include <Nickel2/Core/Assert.hpp>
#include <Nickel2/Core/Utils.hpp>
#include <Nickel2/Core/Logger.hpp>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <map>
#include <unordered_map>
#include <vector>
#include <utility>
#include <future>
#include <thread>
#include <filesystem>