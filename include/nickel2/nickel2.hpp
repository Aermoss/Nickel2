#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define NICKEL2_VERSION "0.0.3"

#define NICKEL2_TRUE 1
#define NICKEL2_FALSE 0

#define NICKEL2_BOOL GL_BOOL
#define NICKEL2_FLOAT GL_FLOAT
#define NICKEL2_INT GL_INT
#define NICKEL2_UNSIGNED_INT GL_UNSIGNED_INT
#define NICKEL2_UNSIGNED_BYTE GL_UNSIGNED_BYTE

#include "color.hpp"
#include "vertex.hpp"
#include "context.hpp"
#include "model.hpp"
#include "mesh.hpp"
#include "window.hpp"
#include "buffer.hpp"
#include "vertexArray.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "transform.hpp"
#include "material.hpp"
#include "renderer.hpp"
#include "scene.hpp"
#include "utils.hpp"

#include "audio/listener.hpp"
#include "audio/source.hpp"