#version 460

in DATA {
    vec3 position;
    vec2 texCoord;
    vec3 normal;
    vec3 cameraPosition;
    mat4 proj;
    mat4 view;
    mat4 model;
} data;

uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D normalMap;
uniform sampler2D ambientMap;
uniform sampler2D specularMap;

uniform vec3 albedoDefault;
uniform float roughnessDefault;
uniform float metallicDefault;
uniform float ambientDefault;

uniform int useAlbedoMap;
uniform int useRoughnessMap;
uniform int useMetallicMap;
uniform int useNormalMap;
uniform int useAmbientMap;
uniform int useSpecularMap;

const int MAX_LIGHTS = 32;
uniform int lightCount;
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];
uniform float lightBrightnesses[MAX_LIGHTS];

uniform int enableIBL;
uniform int enableShadows;
uniform samplerCube depthMap;
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

const float PI = 3.14159265359f;

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a2 = roughness * roughness * roughness * roughness;
    float NdotH = max(dot(N, H), 0.0f);
    float denom = (NdotH * NdotH * (a2 - 1.0f) + 1.0f);
    return a2 / (PI * denom * denom);
}

float geometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;
    return NdotV / (NdotV * (1.0f - k) + k);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    return geometrySchlickGGX(max(dot(N, L), 0.0f), roughness) *
           geometrySchlickGGX(max(dot(N, V), 0.0f), roughness);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0f - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

vec3 getNormalFromMap() {
    vec3 tangentNormal = texture(normalMap, data.texCoord).xyz * 2.0f - 1.0f;

    vec3 Q1 = dFdx(data.position);
    vec3 Q2 = dFdy(data.position);
    vec2 st1 = dFdx(data.texCoord);
    vec2 st2 = dFdy(data.texCoord);

    vec3 N = normalize(data.normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

vec3 gridSamplingDisk[20] = vec3[](
   vec3(1.0f, 1.0f,  1.0f), vec3( 1.0f, -1.0f,  1.0f), vec3(-1.0f, -1.0f,  1.0f), vec3(-1.0f, 1.0f,  1.0f), 
   vec3(1.0f, 1.0f, -1.0f), vec3( 1.0f, -1.0f, -1.0f), vec3(-1.0f, -1.0f, -1.0f), vec3(-1.0f, 1.0f, -1.0f),
   vec3(1.0f, 1.0f,  0.0f), vec3( 1.0f, -1.0f,  0.0f), vec3(-1.0f, -1.0f,  0.0f), vec3(-1.0f, 1.0f,  0.0f),
   vec3(1.0f, 0.0f,  1.0f), vec3(-1.0f,  0.0f,  1.0f), vec3( 1.0f,  0.0f, -1.0f), vec3(-1.0f, 0.0f, -1.0f),
   vec3(0.0f, 1.0f,  1.0f), vec3( 0.0f, -1.0f,  1.0f), vec3( 0.0f, -1.0f, -1.0f), vec3( 0.0f, 1.0f, -1.0f)
);

float farPlane = 1000.0f;

float shadowCalculation(int lightIndex) {
    if (lightIndex != 0 || enableShadows == 0) return 0.0f;

    vec3 fragToLight = data.position - lightPositions[lightIndex];
    float currentDepth = length(fragToLight);
    float shadow = 0.0f;
    float bias = 0.15f;
    int samples = 22;
    float viewDistance = length(data.cameraPosition - data.position);
    float diskRadius = (1.0f + (viewDistance / farPlane)) / 125.0f;

    for (int i = 0; i < samples; ++i) {
        float closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= farPlane;

        if (currentDepth - bias > closestDepth)
            shadow += 1.0f;
    }

    shadow /= float(samples);
    return shadow;
}

void main() {
    vec4 albedo = vec4(albedoDefault, 1.0f);
    float metallic = metallicDefault;
    float roughness = roughnessDefault;
    float ao = ambientDefault;

    if (useAlbedoMap == 1)
        albedo = pow(texture(albedoMap, data.texCoord), vec4(2.2f));

    if (useMetallicMap == 1 && enableIBL == 1)
        metallic = texture(metallicMap, data.texCoord).r;

    if (useRoughnessMap == 1)
        roughness = texture(roughnessMap, data.texCoord).r;

    if (useAmbientMap == 1)
        ao = texture(ambientMap, data.texCoord).r;

    if (albedo.a < 0.1f)
        discard;

    vec3 N = normalize(data.normal);

    if (useNormalMap == 1)
        N = getNormalFromMap();

    vec3 V = normalize(data.cameraPosition - data.position);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04f); 
    F0 = mix(F0, vec3(albedo), metallic);
    vec3 Lo = vec3(0.0f);

    for (int i = 0; i < lightCount; ++i) {
        vec3 L = normalize(lightPositions[i] - data.position);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - data.position);
        float attenuation = (lightBrightnesses[i] * 100.0f) / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;
        float NDF = distributionGGX(N, H, roughness);
        float G = geometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);
        vec3 numerator = NDF * G * F;
        float denominator = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f;
        vec3 specular = numerator / denominator;
        vec3 kS = F;
        vec3 kD = vec3(1.0f) - kS;
        kD *= 1.0f - metallic;
        float NdotL = max(dot(N, L), 0.0f);
        float shadow = shadowCalculation(i);
        Lo += (kD * vec3(albedo) / PI + specular) * (1.0f - shadow) * radiance * NdotL;
    }
     
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0f), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0f - kS;
    kD *= 1.0f - metallic;
    vec3 diffuse = vec3(albedo);
    vec3 specular = vec3(0.0f);

    if (enableIBL == 1) {
        vec3 irradiance = texture(irradianceMap, N).rgb;
        diffuse = irradiance * diffuse;
        const float MAX_REFLECTION_LOD = 4.0f;
        vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
        vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0f), roughness)).rg;
        specular = prefilteredColor * (F * brdf.x + brdf.y);
    }

    vec3 ambient = (kD * diffuse + specular) * ao;
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f / 2.2f)); 
    gl_FragColor = vec4(color, 1.0f);
}