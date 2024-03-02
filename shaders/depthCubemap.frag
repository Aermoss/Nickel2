#version 460

in vec4 fragPos;

uniform vec3 lightPosition;
float farPlane = 1000.0f;

void main() {
    float lightDistance = length(fragPos.xyz - lightPosition);
    lightDistance = lightDistance / farPlane;
    gl_FragDepth = lightDistance;
}