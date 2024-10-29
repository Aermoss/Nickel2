
#version 460

out vec4 fragColor;

in vec2 fragTexCoord;

uniform sampler2D sourceTexture;
uniform float filterRadius;

void main() {
    float x = filterRadius;
    float y = filterRadius;

    vec3 a = texture(sourceTexture, vec2(fragTexCoord.x - x, fragTexCoord.y + y)).rgb;
    vec3 b = texture(sourceTexture, vec2(fragTexCoord.x, fragTexCoord.y + y)).rgb;
    vec3 c = texture(sourceTexture, vec2(fragTexCoord.x + x, fragTexCoord.y + y)).rgb;

    vec3 d = texture(sourceTexture, vec2(fragTexCoord.x - x, fragTexCoord.y)).rgb;
    vec3 e = texture(sourceTexture, vec2(fragTexCoord.x, fragTexCoord.y)).rgb;
    vec3 f = texture(sourceTexture, vec2(fragTexCoord.x + x, fragTexCoord.y)).rgb;

    vec3 g = texture(sourceTexture, vec2(fragTexCoord.x - x, fragTexCoord.y - y)).rgb;
    vec3 h = texture(sourceTexture, vec2(fragTexCoord.x, fragTexCoord.y - y)).rgb;
    vec3 i = texture(sourceTexture, vec2(fragTexCoord.x + x, fragTexCoord.y - y)).rgb;

    vec3 result = e * 4.0f;
    result += (b + d + f + h) * 2.0f;
    result += (a + c + g + i);
    result *= 1.0f / 16.0f;
    result = texture(sourceTexture, fragTexCoord).rgb;
    fragColor = vec4(result, 1.0f);
}