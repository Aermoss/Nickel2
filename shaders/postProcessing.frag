#version 460

out vec4 fragColor;

const float PI = 3.14159265359f;

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
uniform vec2 resolution;
uniform float time;

uniform vec4 vignetteColor;
uniform float vignetteStrength;

void main() {
    vec2 uv = data.texCoord.xy * (1.0f - data.texCoord.yx);
    float vig = pow(uv.x * uv.y * 15.0f, vignetteStrength);
    // fragColor = mix(vignetteColor, texture(albedoMap, data.texCoord), vig);

    // vec2 texCoord = data.texCoord;
    // texCoord.x += sin(texCoord.y * 5.0f * PI + (time * 10.0f)) / 100.0f;
    fragColor = mix(vignetteColor, texture(albedoMap, data.texCoord), vig);
}