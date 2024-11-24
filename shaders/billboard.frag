#version 460

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

in vec2 fragTexCoord;

uniform vec3 color;
uniform sampler2D albedoMap;

void main() {
	fragColor = texture(albedoMap, fragTexCoord) * vec4(color, 1.0f);
}