
#version 460

#define ONE_OVER_4PI 0.0795774715459476f

#define BLEND_START 8
#define BLEND_END 200

out vec4 outColor;

layout (binding = 1) uniform sampler2D perlin;
layout (binding = 2) uniform samplerCube envmap;
layout (binding = 3) uniform sampler2D gradients;

uniform vec4 uvParams;
uniform vec2 perlinOffset;
uniform vec3 oceanColor;

in vec3 vdir;
in vec2 tex;

void main() {
    const vec3 sunColor = vec3(1.0f, 1.0f, 0.47f);
    const vec3 perlinFrequency = vec3(1.12f, 0.59f, 0.23f);
    const vec3 perlinGradient = vec3(0.014f, 0.016f, 0.022f);
    const vec3 sundir = vec3(0.603f, 0.240f, -0.761f);

    float dist = length(vdir.xz);
    float factor = (BLEND_END - dist) / (BLEND_END - BLEND_START);
    vec2 perl = vec2(0.0f);

    factor = clamp(factor * factor * factor, 0.0f, 1.0f);

    if (factor < 1.0f) {
        vec2 ptex = tex + uvParams.zw;
        vec2 p0 = texture(perlin, ptex * perlinFrequency.x + perlinOffset).rg;
        vec2 p1 = texture(perlin, ptex * perlinFrequency.y + perlinOffset).rg;
        vec2 p2 = texture(perlin, ptex * perlinFrequency.z + perlinOffset).rg;
        perl = (p0 * perlinGradient.x + p1 * perlinGradient.y + p2 * perlinGradient.z);
    }

    vec4 grad = texture(gradients, tex);
    grad.xy = mix(perl, grad.xy, factor);

    vec3 n = normalize(grad.xzy);
    vec3 v = normalize(vdir);
    vec3 l = reflect(-v, n);

    float F0 = 0.020018673f;
    float F = F0 + (1.0f - F0) * pow(1.0f - dot(n, l), 5.0f);

    vec3 refl = texture(envmap, l).rgb;

    float turbulence = max(1.6f - grad.w, 0.0f);
    float color_mod = mix(1.0f, 1.0f + 3.0f * smoothstep(1.2f, 1.8f, turbulence), factor);

#if 1
    const float rho = 0.3f;
    const float ax = 0.2f;
    const float ay = 0.1f;

    vec3 h = sundir + v;
    vec3 x = cross(sundir, n);
    vec3 y = cross(x, n);

    float mult = (ONE_OVER_4PI * rho / (ax * ay * sqrt(max(1e-5, dot(sundir, n) * dot(v, n)))));
    float hdotx = dot(h, x) / ax;
    float hdoty = dot(h, y) / ay;
    float hdotn = dot(h, n);
    float spec = mult * exp(-((hdotx * hdotx) + (hdoty * hdoty)) / (hdotn * hdotn));
#else
    float spec = pow(clamp(dot(sundir, l), 0.0f, 1.0f), 400.0f);
#endif

    outColor = vec4(mix(oceanColor, refl * color_mod, F), 1.0f);
}