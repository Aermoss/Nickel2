#version 460

in DATA {
    vec3 position;
    vec2 texCoord;
    vec3 normal;
    vec3 cameraPosition;
    mat4 proj;
    mat4 view;
    mat4 model;
} data_in;

uniform sampler2D albedoMap;
uniform vec3 albedoDefault;
uniform int useAlbedoMap;
uniform int isTransparent;
uniform float transparency;

void main() {
    vec4 albedo = vec4(albedoDefault, 1.0f);

    if (useAlbedoMap == 1)
        albedo = texture(albedoMap, data_in.texCoord);

    if (albedo.a < 0.1f)
        discard;

    if (isTransparent == 1)
        albedo.a = transparency;

    gl_FragColor = albedo;
}