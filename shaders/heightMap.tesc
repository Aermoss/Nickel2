#version 460

layout (vertices = 4) out;

uniform mat3 inverseModel;
uniform mat4 model;
uniform mat4 view;

uniform vec3 cameraPosition;

in vec2 tscTexCoord[];
out vec2 tesTexCoord[];

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tesTexCoord[gl_InvocationID] = tscTexCoord[gl_InvocationID];

    if (gl_InvocationID == 0) {
        const int MIN_TESS_LEVEL = 8;
        const int MAX_TESS_LEVEL = 64;
        const float MIN_DISTANCE = 5000;
        const float MAX_DISTANCE = 10000;

        float eyeSpacePos00 = length(vec3(model * gl_in[0].gl_Position).xz - cameraPosition.xz);
        float eyeSpacePos01 = length(vec3(model * gl_in[1].gl_Position).xz - cameraPosition.xz);
        float eyeSpacePos10 = length(vec3(model * gl_in[2].gl_Position).xz - cameraPosition.xz);
        float eyeSpacePos11 = length(vec3(model * gl_in[3].gl_Position).xz - cameraPosition.xz);

        float distance00 = int(clamp((abs(eyeSpacePos00) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0f, 1.0f) * 10.0f) / 10.0f;
        float distance01 = int(clamp((abs(eyeSpacePos01) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0f, 1.0f) * 10.0f) / 10.0f;
        float distance10 = int(clamp((abs(eyeSpacePos10) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0f, 1.0f) * 10.0f) / 10.0f;
        float distance11 = int(clamp((abs(eyeSpacePos11) - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE), 0.0f, 1.0f) * 10.0f) / 10.0f;
        
        float tessLevel0 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00));
        float tessLevel1 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01));
        float tessLevel2 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11));
        float tessLevel3 = mix(MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10));

        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel1;
        gl_TessLevelOuter[2] = tessLevel2;
        gl_TessLevelOuter[3] = tessLevel3;
        gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
        gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
    }
}