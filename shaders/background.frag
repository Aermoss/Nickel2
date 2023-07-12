#version 460

in vec3 fragPosition;

uniform samplerCube environmentMap;

void main() {
    vec3 envColor = texture(environmentMap, fragPosition).rgb;
    envColor = envColor / (envColor + vec3(1.0f));
    envColor = pow(envColor, vec3(1.0f / 2.2f)); 
    gl_FragColor = vec4(envColor, 1.0f);
}