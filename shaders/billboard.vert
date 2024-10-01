#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 view;
uniform mat4 proj;

uniform vec3 pos;
uniform vec3 up;
uniform vec3 right;
uniform vec3 scale;

out vec2 fragTexCoord;

void main() {
    vec3 vPos = position * scale;
    gl_Position = proj * view * vec4(pos + right * vPos.x * + up * vPos.y, 1.0f);
    gl_Position /= gl_Position.w;
    gl_Position.xy += vPos.xy * vec2(0.5f, 0.5f);
    fragTexCoord = texCoord;
}