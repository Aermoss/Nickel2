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
uniform sampler2D bloomBlur;
uniform vec2 resolution;
uniform float time;

uniform vec2 velocity;
uniform vec4 vignetteColor;
uniform float vignetteStrength;
uniform float exposure = 1.0f;
uniform float gamma = 2.2f;

uniform int enableMotionBlur = 0;

void main() {
    vec2 uv = data.texCoord.xy * (1.0f - data.texCoord.yx);
    float vignette = pow(uv.x * uv.y * 15.0f, vignetteStrength);

    vec2 texCoord = data.texCoord;
    // texCoord.x += sin(texCoord.y * 5.0f * PI + (time * 2.0f)) / 100.0f;

    vec4 color = texture(albedoMap, texCoord);

    if (enableMotionBlur == 1) {
        const int numSamples = 8;

        for (int i = 1; i < numSamples; ++i, texCoord += velocity / 250.0f)
            color += texture(albedoMap, texCoord);

        color /= numSamples;
    }

    vec3 result = vec3(color) + texture(bloomBlur, data.texCoord).rgb;
    result = vec3(1.0f) - exp(-result * exposure);
    result = pow(result, vec3(1.0f / gamma));
    fragColor = mix(vignetteColor, vec4(result, 1.0f), vignette);
}