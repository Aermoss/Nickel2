#pragma once

#include <Nickel2/Core/Platform.hpp>
#include <Nickel2/Core/Base.hpp>
#include <Nickel2/Core/Assert.hpp>
#include <Nickel2/Core/Constants.hpp>
#include <Nickel2/Core/Utils.hpp>
#include <Nickel2/Core/Logger.hpp>

#ifdef NICKEL2_PLATFORM_WINDOWS
#include <windows.h>
#include <dwmapi.h>
#endif

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
#include <filesystem>
#include <thread>

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
#define GLFW_NATIVE_INCLUDE_NONE
#include <glfw/glfw3native.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <imguizmo/ImGuizmo.h>

#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <stb/stb_image.h>
#include <entt/entt.hpp>
#include <rvr/rvr.hpp>