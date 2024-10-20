#version 460

out vec4 fragColor;
in vec3 fragPosition;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591f, 0.3183f);

vec2 SampleSphericalMap(vec3 v) {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    vec2 uv = SampleSphericalMap(normalize(fragPosition));
    vec3 color = texture(equirectangularMap, uv).rgb;
    fragColor = vec4(color, 1.0f);
}