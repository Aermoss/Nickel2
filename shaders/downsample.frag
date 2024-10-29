
#version 460

out vec4 fragColor;

in vec2 fragTexCoord;

uniform sampler2D sourceTexture;
uniform vec2 sourceResolution;
uniform int mipLevel = 1;

vec3 PowVec3(vec3 v, float p) {
    return vec3(pow(v.x, p), pow(v.y, p), pow(v.z, p));
}

const float invGamma = 1.0f / 2.2f;
vec3 ToSRGB(vec3 v) { return PowVec3(v, invGamma); }

float sRGBToLuma(vec3 col) {
    return dot(col, vec3(0.299f, 0.587f, 0.114f));
}

float KarisAverage(vec3 col) {
    float luma = sRGBToLuma(ToSRGB(col)) * 0.25f;
    return 1.0f / (1.0f + luma);
}

void main() {
    vec2 srcTexelSize = 1.0f / sourceResolution;
    float x = srcTexelSize.x, y = srcTexelSize.y;

    vec3 a = texture(sourceTexture, vec2(fragTexCoord.x - 2 * x, fragTexCoord.y + 2 * y)).rgb;
    vec3 b = texture(sourceTexture, vec2(fragTexCoord.x, fragTexCoord.y + 2 * y)).rgb;
    vec3 c = texture(sourceTexture, vec2(fragTexCoord.x + 2 * x, fragTexCoord.y + 2 * y)).rgb;

    vec3 d = texture(sourceTexture, vec2(fragTexCoord.x - 2 * x, fragTexCoord.y)).rgb;
    vec3 e = texture(sourceTexture, vec2(fragTexCoord.x, fragTexCoord.y)).rgb;
    vec3 f = texture(sourceTexture, vec2(fragTexCoord.x + 2 * x, fragTexCoord.y)).rgb;

    vec3 g = texture(sourceTexture, vec2(fragTexCoord.x - 2 * x, fragTexCoord.y - 2 * y)).rgb;
    vec3 h = texture(sourceTexture, vec2(fragTexCoord.x, fragTexCoord.y - 2 * y)).rgb;
    vec3 i = texture(sourceTexture, vec2(fragTexCoord.x + 2 * x, fragTexCoord.y - 2 * y)).rgb;

    vec3 j = texture(sourceTexture, vec2(fragTexCoord.x - x, fragTexCoord.y + y)).rgb;
    vec3 k = texture(sourceTexture, vec2(fragTexCoord.x + x, fragTexCoord.y + y)).rgb;
    vec3 l = texture(sourceTexture, vec2(fragTexCoord.x - x, fragTexCoord.y - y)).rgb;
    vec3 m = texture(sourceTexture, vec2(fragTexCoord.x + x, fragTexCoord.y - y)).rgb;

    vec3 result, groups[5];

    switch (mipLevel) {
        case 0: {
            groups[0] = (a+b+d+e) * (0.125f / 4.0f);
            groups[1] = (b+c+e+f) * (0.125f / 4.0f);
            groups[2] = (d+e+g+h) * (0.125f / 4.0f);
            groups[3] = (e+f+h+i) * (0.125f / 4.0f);
            groups[4] = (j+k+l+m) * (0.5f / 4.0f);
            groups[0] *= KarisAverage(groups[0]);
            groups[1] *= KarisAverage(groups[1]);
            groups[2] *= KarisAverage(groups[2]);
            groups[3] *= KarisAverage(groups[3]);
            groups[4] *= KarisAverage(groups[4]);
            result = groups[0]+groups[1]+groups[2]+groups[3]+groups[4];
            result = max(result, 0.0001f);
            break;
        } default: {
            result = e * 0.125f;
            result += (a + c + g + i) * 0.03125f;
            result += (b + d + f + h) * 0.0625f;
            result += (j + k + l + m) * 0.125f;
            break;
        }
    }

    fragColor = vec4(result, 1.0f);
}