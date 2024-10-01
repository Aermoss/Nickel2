#version 460

layout (location = 0) in vec3 position;

out vec3 fragPosition;

uniform mat4 proj;
uniform mat4 view;

void main() {
    fragPosition = position;
    gl_Position =  proj * view * vec4(position, 1.0);
}