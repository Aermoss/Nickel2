
#version 460

#define BLEND_START 8
#define BLEND_END 200

in vec3 my_Position;

layout (binding = 0) uniform sampler2D displacement;
layout (binding = 1) uniform sampler2D perlin;

uniform mat4 matLocal;
uniform mat4 matWorld;
uniform mat4 matViewProj;
uniform vec4 uvParams;
uniform vec2 perlinOffset;
uniform vec3 eyePos;

out vec3 vdir;
out vec2 tex;

void main() {
    const vec3 perlinFrequency = vec3(1.12f, 0.59f, 0.23f);
    const vec3 perlinAmplitude = vec3(0.35f, 0.42f, 0.57f);

    vec4 posLocal = matLocal * vec4(my_Position, 1.0f);
    vec2 uvLocal = posLocal.xy * uvParams.x + vec2(uvParams.y);
    vec3 disp = texture(displacement, uvLocal).xyz;

    posLocal = matWorld * posLocal;
    vdir = eyePos - posLocal.xyz;
    tex = uvLocal;

    float dist = length(vdir.xz);
    float factor = clamp((BLEND_END - dist) / (BLEND_END - BLEND_START), 0.0f, 1.0f);
    float perl = 0.0f;

    if (factor < 1.0f) {
        vec2 ptex = uvLocal + uvParams.zw;
        float p0 = texture(perlin, ptex * perlinFrequency.x + perlinOffset).a;
        float p1 = texture(perlin, ptex * perlinFrequency.y + perlinOffset).a;
        float p2 = texture(perlin, ptex * perlinFrequency.z + perlinOffset).a;
        perl = dot(vec3(p0, p1, p2), perlinAmplitude);
    }

    disp = mix(vec3(0.0f, perl, 0.0f), disp, factor);
    gl_Position = matViewProj * vec4(posLocal.xyz + disp, 1.0f);
}