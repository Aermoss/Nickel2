#version 460

out vec4 fragColor;
in vec3 fragPosition;

uniform samplerCube environmentMap;

void main() {
    vec3 envColor = texture(environmentMap, fragPosition).rgb;
    // envColor = envColor / (envColor + vec3(1.0f));
    // envColor = pow(envColor, vec3(1.0f / 2.2f)); 
    fragColor = vec4(envColor, 1.0f);
}