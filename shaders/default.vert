#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out DATA {
    vec3 position;
    vec2 texCoord;
    vec3 normal;
    vec3 cameraPosition;
    mat4 proj;
    mat4 view;
    mat4 model;
} data;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;
uniform mat3 modelMatrix;

uniform vec3 cameraPosition;

uniform int textureScale;
uniform int rotateTexture;
uniform int inverseNormal;

void main() {
    data.position = vec3(model * vec4(position, 1.0f));
    data.texCoord = texCoord;

    if (textureScale != 0)
        data.texCoord *= textureScale;

    if (rotateTexture == 1)
        data.texCoord = vec2(data.texCoord.y, data.texCoord.x);

    if (inverseNormal == 1)
        data.normal = vec3(modelMatrix * -normal);

    else
        data.normal = vec3(modelMatrix * normal);
    
    data.cameraPosition = cameraPosition;
    data.proj = proj;
    data.view = view;
    data.model = model;
    gl_Position = proj * view * vec4(data.position, 1.0f);
}