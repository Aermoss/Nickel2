#version 460

const float PI = 3.14159265359f;

out vec4 fragColor;
in vec3 fragPosition;

uniform samplerCube environmentMap;

void main() {
    vec3 N = normalize(fragPosition);
    vec3 irradiance = vec3(0.0f);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = normalize(cross(up, N));
    up = normalize(cross(N, right));
       
    float sampleDelta = 0.025f;
    float nrSamples = 0.0f;

    for (float phi = 0.0f; phi < 2.0f * PI; phi += sampleDelta) {
        for (float theta = 0.0f; theta < 0.5f * PI; theta += sampleDelta) {
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = normalize(tangentSample.x * right + tangentSample.y * up + tangentSample.z * N);
            vec3 texColor = texture(environmentMap, sampleVec).rgb;
            irradiance += texColor * cos(theta) * sin(theta);
            nrSamples++;
        }
    }

    if (nrSamples != 0.0f)
        irradiance = PI * irradiance * (1.0f / float(nrSamples));

    fragColor = vec4(irradiance, 1.0f);
}