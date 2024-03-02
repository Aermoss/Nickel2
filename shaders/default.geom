#version 460

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in DATA {
    vec3 position;
    vec4 lightSpacePosition;
    vec2 texCoord;
    vec3 normal;
    vec3 cameraPosition;
    mat4 proj;
    mat4 view;
    mat4 model;
} data_in[];

out DATA {
    vec3 position;
    vec4 lightSpacePosition;
    vec2 texCoord;
    vec3 normal;
    vec3 cameraPosition;
    mat4 proj;
    mat4 view;
    mat4 model;
} data_out;

void main() {
    gl_Position = data_in[0].proj * data_in[0].view * gl_in[0].gl_Position;
    data_out.position = gl_in[0].gl_Position.xyz;
    data_out.texCoord = data_in[0].texCoord;
    data_out.normal = data_in[0].normal;
    data_out.cameraPosition = data_in[0].cameraPosition;
    EmitVertex();

    gl_Position = data_in[1].proj * data_in[1].view * gl_in[1].gl_Position;
    data_out.position = gl_in[1].gl_Position.xyz;
    data_out.texCoord = data_in[1].texCoord;
    data_out.normal = data_in[1].normal;
    data_out.cameraPosition = data_in[1].cameraPosition;
    EmitVertex();

    gl_Position = data_in[2].proj * data_in[2].view * gl_in[2].gl_Position;
    data_out.position = gl_in[2].gl_Position.xyz;
    data_out.texCoord = data_in[2].texCoord;
    data_out.normal = data_in[2].normal;
    data_out.cameraPosition = data_in[2].cameraPosition;
    EmitVertex();

    EndPrimitive();
}