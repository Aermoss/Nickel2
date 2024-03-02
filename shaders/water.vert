#version 460

layout (location = 0) in vec2 position;

out vec2 fragTexCoord;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main(void) {
    fragTexCoord = vec2(position.x / 2.0f + 0.5f, position.y / 2.0f + 0.5f);
    gl_Position = proj * view * model * vec4(position.x, 0.0f, position.y, 1.0f);
}