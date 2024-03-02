#version 460

out vec4 fragColor;

in DATA {
    vec3 position;
    vec4 worldSpacePosition;
    vec4 lightSpacePosition;
    vec2 texCoord;
    vec3 normal;
    vec3 cameraPosition;
    mat4 proj;
    mat4 view;
    mat4 model;
} data;

uniform sampler2D albedoMap;
uniform sampler2D roughnessMap;
uniform sampler2D metallicMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform sampler2D ambientMap;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

uniform vec4 albedoDefault;
uniform float roughnessDefault;
uniform float metallicDefault;
uniform float ambientDefault;

uniform int useAlbedoMap;
uniform int useRoughnessMap;
uniform int useMetallicMap;
uniform int useNormalMap;
uniform int useSpecularMap;
uniform int useAmbientMap;

const int MAX_POINT_LIGHTS = 32;
uniform int pointLightCount;
uniform vec3 pointLightPositions[MAX_POINT_LIGHTS];
uniform vec3 pointLightColors[MAX_POINT_LIGHTS];
uniform float pointLightBrightnesses[MAX_POINT_LIGHTS];

uniform int enableDirectionalLight;
uniform vec3 directionalLightPosition;

uniform samplerCube depthMapPoint;
uniform sampler2D depthMapDirectional;

uniform int enableIBL;
uniform int enableSSAO;
uniform int enableShadows;
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;
uniform sampler2D ssaoBlur;

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

vec3 getNormalFromMap(vec3 position, vec3 normal) {
    vec3 tangentNormal = texture(normalMap, data.texCoord).xyz * 2.0f - 1.0f;

    vec3 Q1 = dFdx(position);
    vec3 Q2 = dFdy(position);
    vec2 st1 = dFdx(data.texCoord);
    vec2 st2 = dFdy(data.texCoord);

    vec3 N = normalize(normal);
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

float shadowCalculationPoint(int lightIndex, vec3 position) {
    if (lightIndex != 0 || enableShadows == 0) return 0.0f;
    vec3 fragToLight = position - pointLightPositions[lightIndex];
    float currentDepth = length(fragToLight);
    float shadow = 0.0f;
    float bias = 0.15f;
    int samples = 22;
    float viewDistance = length(data.cameraPosition - position);
    float diskRadius = (1.0f + (viewDistance / farPlane)) / 125.0f;

    for (int i = 0; i < samples; ++i) {
        float closestDepth = texture(depthMapPoint, fragToLight + gridSamplingDisk[i] * diskRadius).r * farPlane;
        if (currentDepth - bias > closestDepth) shadow += 1.0f;
    }

    shadow /= float(samples);
    return shadow;
}

float shadowCalculationDirectional(vec3 normal) {
    if (enableShadows == 0) return 0.0f;
    vec3 lightCoords = data.lightSpacePosition.xyz / data.lightSpacePosition.w;
    float shadow = 0.0f;

    if (lightCoords.z <= 1.0f) {
        lightCoords = (lightCoords + 1.0f) / 2.0f;
        float currentDepth = lightCoords.z;
        float bias = max(0.025f * (1.0f - dot(normal, normalize(directionalLightPosition))), 0.0005f);

        int sampleRadius = 2;
        vec2 pixelSize = 1.0f / textureSize(depthMapDirectional, 0);

        for (int y = -sampleRadius; y <= sampleRadius; y++) {
            for (int x = -sampleRadius; x <= sampleRadius; x++) {
                float closestDepth = texture(depthMapDirectional, lightCoords.xy + vec2(x, y) * pixelSize).r;
                if (currentDepth > closestDepth + bias) shadow += 1.0f;
            }
        } shadow /= pow((sampleRadius * 2 + 1), 2);
    } return shadow;
}

void main() {
    vec2 screenSpaceUV = (data.worldSpacePosition.xy / data.worldSpacePosition.w) * 0.5f + 0.5f;
    float ssaoFactor = texture(ssaoBlur, screenSpaceUV).r;

#if 0
    vec3 position = texture(gPosition, screenSpaceUV).xyz;
    vec3 normal = texture(gNormal, screenSpaceUV).xyz;
#else
    vec3 position = data.position;
    vec3 normal = data.normal;
#endif

    vec4 albedo = albedoDefault;
    float metallic = metallicDefault;
    float roughness = roughnessDefault;
    float ambientFactor = 3.0f;
    float ambient = enableSSAO == 1 ? ssaoFactor / ambientFactor : 1.0f / ambientFactor;

    if (useAlbedoMap == 1) {
        vec4 color = texture(albedoMap, data.texCoord);
        albedo = vec4(pow(color.rgb, vec3(2.2f)), color.a);
    }

#if 0
    if (useMetallicMap == 1 && enableIBL == 1)
        metallic = texture(metallicMap, data.texCoord).r;

    if (useAmbientMap == 1)
        ambient *= texture(ambientMap, data.texCoord).r;

    if (useRoughnessMap == 1)
        roughness = texture(roughnessMap, data.texCoord).r;
#else
    if (useAmbientMap == 1)
        ambient *= texture(ambientMap, data.texCoord).r;

    if (useMetallicMap == 1 && enableIBL == 1)
        metallic = texture(metallicMap, data.texCoord).r;

    if (useRoughnessMap == 1) {
        vec4 roughnessTexture = texture(roughnessMap, data.texCoord);
        ambient *= roughnessTexture.r;
        roughness = roughnessTexture.g;
        metallic = roughnessTexture.b;
    }
#endif

    if (albedo.a < 0.01f)
        discard;

    vec3 N = normalize(normal);

    if (useNormalMap == 1)
        N = getNormalFromMap(position, normal);

    vec3 V = normalize(data.cameraPosition - position);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04f); 
    F0 = mix(F0, vec3(albedo), metallic);
    vec3 Lo = vec3(0.0f);

    if (enableDirectionalLight == 1) {
        vec3 L = normalize(directionalLightPosition);
        vec3 H = normalize(V + L);
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
        float shadow = shadowCalculationDirectional(normal);
        Lo += (kD * vec3(albedo) / PI + specular) * (1.0f - shadow) * 10.0f * NdotL;
    }

    for (int i = 0; i < pointLightCount; i++) {
        vec3 L = normalize(pointLightPositions[i] - position);
        vec3 H = normalize(V + L);
        float distance = length(pointLightPositions[i] - position);
        float attenuation = (pointLightBrightnesses[i] * 100.0f) / (distance * distance);
        vec3 radiance = pointLightColors[i] * attenuation;
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
        float shadow = shadowCalculationPoint(i, position);
        Lo += (kD * vec3(albedo) / PI + specular) * (1.0f - shadow) * radiance * NdotL;
    }

    Lo = clamp(Lo, 0.0f, 1.0f);

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

    vec3 ao = (kD * diffuse + specular) * ambient;
    vec3 color = ao + Lo;
    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f / 2.2f));
    fragColor = vec4(color, albedo.a);
}