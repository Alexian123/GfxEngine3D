#ifndef LIGHTING_GLSL
#define LIGHTING_GLSL

#include "lighting.glsl"

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

vec3 calculateDirLight(DirLight light, Material material, vec3 normal, vec3 viewDir, vec2 texCoord);
vec3 calculatePointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord);
vec3 calculateSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord);

vec3 calculateDirLight(DirLight light, Material material, vec3 normal, vec3 viewDir, vec2 texCoord)
{
	vec3 lightDir = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// combine results
	vec3 ambient = light.color.ambient * vec3(texture(material.diffuse, texCoord));
	vec3 diffuse = light.color.diffuse * diff * vec3(texture(material.diffuse, texCoord));
	vec3 specular = light.color.specular * spec * vec3(texture(material.specular, texCoord));

	return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord)
{
	vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // combine results
    vec3 ambient = light.color.ambient * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse = light.color.diffuse * diff * vec3(texture(material.diffuse, texCoord));
    vec3 specular = light.color.specular * spec * vec3(texture(material.specular, texCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord)
{
	vec3 lightDir = normalize(light.pl.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// intenisty
	float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // attenuation
    float distance = length(light.pl.position - fragPos);
    float attenuation = 1.0 / (light.pl.constant + light.pl.linear * distance + light.pl.quadratic * (distance * distance));    

    // combine results
    vec3 ambient = light.pl.color.ambient * vec3(texture(material.diffuse, texCoord));
    vec3 diffuse = light.pl.color.diffuse * diff * vec3(texture(material.diffuse, texCoord));
    vec3 specular = light.pl.color.specular * spec * vec3(texture(material.specular, texCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}

#endif