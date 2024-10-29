#version 460

out vec4 fragColor;

in vec2 fragTexCoord;

uniform vec3 color;
uniform sampler2D albedoMap;

void main() {
	fragColor = texture(albedoMap, fragTexCoord) * vec4(color, 1.0f);
}