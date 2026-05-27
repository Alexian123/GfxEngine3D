#version 330 core

#include "common/lighting.glsl"

#define MAX_NUM_DIR_LIGHTS  4
#define MAX_NUM_POINT_LIGHTS 8
#define MAX_NUM_SPOT_LIGHTS 2

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vWorldPos;

out vec4 FragColor;

uniform Material uMaterial;
uniform vec3 uViewPos;
uniform DirLight uDirLights[MAX_NUM_DIR_LIGHTS];
uniform PointLight uPointLights[MAX_NUM_POINT_LIGHTS];
uniform SpotLight uSpotLights[MAX_NUM_SPOT_LIGHTS];
uniform uint uNumLightsMask; // b[0:7] = numDirLights, b[8:15] = numPointLights, b[16:23] = numSpotLights

void main()
{
    Surface surface = createSurface(uMaterial, vTexCoord, vNormal, vWorldPos, uViewPos);

    int numDirLights = int(uNumLightsMask & 0xFFu);
    int numPointLights = int((uNumLightsMask >> 8) & 0xFFu);
    int numSpotLights = int((uNumLightsMask >> 16) & 0xFFu);

    // directional lights
    vec3 result = vec3(0.0);
    for (int i = 0; i < MAX_NUM_DIR_LIGHTS; ++i) {
        if (i >= numDirLights) break;
        result += calculateDirLight(uDirLights[i], surface);
    }

    // point lights
    for (int i = 0; i < MAX_NUM_POINT_LIGHTS; ++i) {
        if (i >= numPointLights) break;
        result += calculatePointLight(uPointLights[i], surface);
    }

    // spot lights
    for (int i = 0; i < MAX_NUM_SPOT_LIGHTS; ++i) {
        if (i >= numSpotLights) break;
        result += calculateSpotLight(uSpotLights[i], surface);
    }

    // self light (emission)
    result += surface.emissionColor;

    FragColor = vec4(result, 1.0);
}