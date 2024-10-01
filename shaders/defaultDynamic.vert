#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in ivec4 boneIds;
layout (location = 6) in vec4 weights;

out DATA {
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

uniform mat4 lightSpaceMatrix;
uniform mat3 inverseModel;
uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;

uniform vec3 cameraPosition;

uniform int textureScale;
uniform int rotateTexture;
uniform int inverseNormal;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main() {
    vec4 totalPosition = vec4(0.0f);

    for (int i = 0 ; i < MAX_BONE_INFLUENCE ; i++) {
        if (boneIds[i] == -1)
            continue;

        if (boneIds[i] >= MAX_BONES) {
            totalPosition = vec4(position, 1.0f);
            break;
        }

        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(position, 1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normal;
    }

    data.position = vec3(model * (vec4(position, 1.0f) * totalPosition));
    data.worldSpacePosition = proj * view * vec4(data.position, 1.0f);
    data.lightSpacePosition = lightSpaceMatrix * vec4(data.position, 1.0f);
    data.texCoord = texCoord;

    if (textureScale != 0)
        data.texCoord *= textureScale;

    if (rotateTexture == 1)
        data.texCoord = vec2(data.texCoord.y, data.texCoord.x);

    if (inverseNormal == 1)
        data.normal = vec3(inverseModel * -normal);

    else
        data.normal = vec3(inverseModel * normal);
    
    data.model = model;
    data.proj = proj;
    data.view = view;
    data.cameraPosition = cameraPosition;
    gl_Position = data.worldSpacePosition;
}