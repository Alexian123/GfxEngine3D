#version 330 core

#define MAX_NUM_POINT_LIGHTS 10
#define MAX_NUM_SPOT_LIGHTS 5

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct LightColor
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct DirLight
{
	LightColor color;
	vec3 direction;
};

struct PointLight
{
	LightColor color;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	PointLight pl;
	vec3 direction;
	float cutOff;
	float outerCutOff;
};

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

vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(vNormal);
	vec3 viewDir = normalize(uViewPos - vWorldPos);
    uint numPointLights = uNumLightsMask & 0xFFu;
    uint numSpotLights = (uNumLightsMask >> 8) & 0xFFu;

	// directional lighting
    vec3 result = calculateDirLight(uDirLight, norm, viewDir);

    // point lights
    for (int i = 0; i < int(numPointLights); ++i) {
        result += calculatePointLight(uPointLights[i], norm, vWorldPos, viewDir);  
	}
		
    // spot lights
    for (int i = 0; i < int(numSpotLights); ++i) {
        result += calculateSpotLight(uSpotLights[i], norm, vWorldPos, viewDir);
    }

	// self light (emission)
	vec3 emission = texture(uMaterial.emission, vTexCoord).rgb;

	FragColor = vec4(result + emission, 1.0);
}

vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);

	// combine results
	vec3 ambient = light.color.ambient * vec3(texture(uMaterial.diffuse, vTexCoord));
	vec3 diffuse = light.color.diffuse * diff * vec3(texture(uMaterial.diffuse, vTexCoord));
	vec3 specular = light.color.specular * spec * vec3(texture(uMaterial.specular, vTexCoord));

	return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // combine results
    vec3 ambient = light.color.ambient * vec3(texture(uMaterial.diffuse, vTexCoord));
    vec3 diffuse = light.color.diffuse * diff * vec3(texture(uMaterial.diffuse, vTexCoord));
    vec3 specular = light.color.specular * spec * vec3(texture(uMaterial.specular, vTexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.pl.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);

	// intenisty
	float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // attenuation
    float distance = length(light.pl.position - fragPos);
    float attenuation = 1.0 / (light.pl.constant + light.pl.linear * distance + light.pl.quadratic * (distance * distance));    

    // combine results
    vec3 ambient = light.pl.color.ambient * vec3(texture(uMaterial.diffuse, vTexCoord));
    vec3 diffuse = light.pl.color.diffuse * diff * vec3(texture(uMaterial.diffuse, vTexCoord));
    vec3 specular = light.pl.color.specular * spec * vec3(texture(uMaterial.specular, vTexCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}