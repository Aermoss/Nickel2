#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 tscTexCoord;

void main() {
    tscTexCoord = texCoord;
    gl_Position = vec4(position * 10.0f, 1.0f);
}