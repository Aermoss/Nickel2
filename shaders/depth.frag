#version 460

in vec4 FragPos;

uniform vec3 lightPosition;
float far_plane = 1000.0f;

void main() {
    float lightDistance = length(FragPos.xyz - lightPosition);
    lightDistance = lightDistance / far_plane;
    gl_FragDepth = lightDistance;
}