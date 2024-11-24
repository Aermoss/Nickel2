#version 460

out vec4 fragColor;

in vec3 fragPosition;

uniform sampler2D equirectangularMap;
const vec2 invAtan = vec2(0.1591f, 0.3183f);

vec2 SampleSphericalMap(vec3 v) {
    return vec2(atan(v.z, v.x), asin(v.y)) * invAtan + 0.5f;
}

void main() {
    vec3 color = texture(equirectangularMap, SampleSphericalMap(normalize(fragPosition))).rgb;
    if (any(isinf(color))) { color = vec3(1.0f); }
    if (any(isnan(color))) { color = vec3(0.0f); }
    fragColor = vec4(color, 1.0f);
}