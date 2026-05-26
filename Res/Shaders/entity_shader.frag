#version 330 core

#include "common/lighting.glsl"
#include "common/lighting.glsl"
#include "common/lighting.glsl"

#define MAX_NUM_POINT_LIGHTS 10
#define MAX_NUM_SPOT_LIGHTS 5

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vWorldPos;

out vec4 FragColor;

uniform Material uMaterial;
uniform vec3 uViewPos;
uniform DirLight uDirLight;
uniform PointLight uPointLights[MAX_NUM_POINT_LIGHTS];
uniform SpotLight uSpotLights[MAX_NUM_SPOT_LIGHTS];
uniform uint uNumLightsMask;	// b[0:7] = numPointLights, b[8:15] = numSpotLights

void main()
{
	vec3 norm = normalize(vNormal);
	vec3 viewDir = normalize(uViewPos - vWorldPos);
    uint numPointLights = uNumLightsMask & 0xFFu;
    uint numSpotLights = (uNumLightsMask >> 8) & 0xFFu;

	// directional lighting
    vec3 result = calculateDirLight(uDirLight, uMaterial, norm, viewDir, vTexCoord);

    // point lights
    for (int i = 0; i < int(numPointLights); ++i) {
        result += calculatePointLight(uPointLights[i], uMaterial, norm, vWorldPos, viewDir, vTexCoord);  
	}
		
    // spot lights
    for (int i = 0; i < int(numSpotLights); ++i) {
        result += calculateSpotLight(uSpotLights[i], uMaterial, norm, vWorldPos, viewDir, vTexCoord);
    }

	// self light (emission)
	vec3 emission = texture(uMaterial.emission, vTexCoord).rgb;

	FragColor = vec4(result + emission, 1.0);
}