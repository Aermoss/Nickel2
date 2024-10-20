#version 460

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

in DATA {
    vec3 position;
    vec4 worldSpacePosition;
    vec4 lightSpacePosition;
    vec2 texCoord;
    vec3 normal;
    vec3 cameraPosition;
    mat4 proj;
    mat4 view;
    mat4 model;
} data;

uniform sampler2D albedoMap;
uniform vec3 albedoDefault;
uniform int useAlbedoMap;
uniform int isTransparent;
uniform float transparency;

void main() {
    vec4 albedo = vec4(albedoDefault, 1.0f);

    if (useAlbedoMap == 1)
        albedo = texture(albedoMap, data.texCoord);

    if (albedo.a < 0.1f)
        discard;

    if (isTransparent == 1)
        albedo.a = transparency;

    float brightness = dot(albedo.xyz, vec3(0.2126f, 0.7152f, 0.0722f));
    brightColor = brightness > 1.0f ? vec4(albedo.xyz, 1.0f) : vec4(0.0f, 0.0f, 0.0f, 1.0f);
    fragColor = albedo;
}