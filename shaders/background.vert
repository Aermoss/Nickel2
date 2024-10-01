#version 460

layout (location = 0) in vec3 position;

uniform mat4 proj;
uniform mat4 view;

out vec3 fragPosition;

void main() {
    fragPosition = position;
    mat4 rotView = mat4(mat3(view));
    vec4 clipPos = proj * rotView * vec4(position, 1.0);
    gl_Position = clipPos.xyww;
}