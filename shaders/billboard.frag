#version 460

out vec4 fragColor;

uniform vec3 color;
uniform sampler2D albedoMap;

in vec2 fragTexCoord;

void main() {
	fragColor = texture(albedoMap, fragTexCoord) * vec4(color, 1.0f);
}