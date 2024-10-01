#version 460

out vec4 fragColor;

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

    fragColor = albedo;
}