#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec3 fragWorldPos;
out vec3 fragViewPos;
out vec2 fragTexCoord;
out vec3 fragNormal;

uniform mat3 inverseModel;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    fragWorldPos = vec3(model * vec4(position, 1.0f));
    fragViewPos = vec3(view * vec4(fragWorldPos, 1.0f));
    fragTexCoord = texCoord;
    fragNormal = vec3(inverseModel * normal);
    gl_Position = proj * vec4(fragViewPos, 1.0f);
}