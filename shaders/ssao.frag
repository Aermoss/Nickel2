#version 460

out vec4 fragColor;

in vec2 fragTexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec2 windowSize;
uniform vec3 samples[64];

int kernelSize = 64;
float radius = 0.5f;
float bias = 0.025f;

uniform mat4 proj;

void main() {
    vec2 noiseScale = vec2(windowSize.x / 4.0f, windowSize.y / 4.0f);
    vec3 fragPos = texture(gPosition, fragTexCoord).xyz;
    vec3 normal = normalize(texture(gNormal, fragTexCoord).rgb);
    vec3 randomVec = normalize(texture(texNoise, fragTexCoord * noiseScale).xyz);
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    float occlusion = 0.0f;

    for (int i = 0; i < kernelSize; ++i) {
        vec3 samplePos = TBN * samples[i];
        samplePos = fragPos + samplePos * radius; 

        vec4 offset = vec4(samplePos, 1.0f);
        offset = proj * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5f + 0.5f;

        float sampleDepth = texture(gPosition, offset.xy).z;
        float rangeCheck = smoothstep(0.0f, 1.0f, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0f : 0.0f) * rangeCheck;           
    }

    occlusion = 1.0f - (occlusion / kernelSize);
    fragColor = vec4(vec3(occlusion), 1.0f);
}