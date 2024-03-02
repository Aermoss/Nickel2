#version 460

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;

in vec3 fragWorldPos;
in vec3 fragViewPos;
in vec2 fragTexCoords;
in vec3 fragNormal;

uniform sampler2D albedoMap;

void main() {
    gPosition = vec4(fragViewPos, 1.0f);
    gNormal = vec4(fragNormal, 1.0f);
    gAlbedo = vec4(1.0f);
}