#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define NICKEL2_ARRAY_BUFFER GL_ARRAY_BUFFER
#define NICKEL2_ELEMENT_ARRAY_BUFFER GL_ELEMENT_ARRAY_BUFFER

#define NICKEL2_TRUE 1
#define NICKEL2_FALSE 0

#define NICKEL2_BOOL GL_BOOL
#define NICKEL2_FLOAT GL_FLOAT
#define NICKEL2_INT GL_INT
#define NICKEL2_UNSIGNED_INT GL_UNSIGNED_INT
#define NICKEL2_UNSIGNED_BYTE GL_UNSIGNED_BYTE

#include "window.hpp"
#include "buffer.hpp"
#include "vertexArray.hpp"
#include "vertex.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "model.hpp"
#include "mesh.hpp"
#include "transform.hpp"
#include "material.hpp"
#include "color.hpp"
#include "utils.hpp"