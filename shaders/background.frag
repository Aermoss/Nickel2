#version 460

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

in vec3 fragPosition;

uniform samplerCube environmentMap;
uniform float brightnessFactor = 1.0f;

void main() {
    vec3 color = texture(environmentMap, fragPosition).rgb;
    float brightness = dot(color, vec3(0.2126f, 0.7152f, 0.0722f)) * brightnessFactor;
    brightColor = brightness > 1.0f ? vec4(color, 1.0f) : vec4(0.0f, 0.0f, 0.0f, 1.0f);
    fragColor = vec4(color, 1.0f);
}