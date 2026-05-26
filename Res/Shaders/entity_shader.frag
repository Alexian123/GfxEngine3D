#version 330 core

#define MAX_NUM_LIGHTS 10

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vWorldPos;

out vec4 FragColor;

uniform Material uMaterial;
uniform int uNumLights;
uniform Light uLights[MAX_NUM_LIGHTS];
uniform vec3 uViewPos;

void main()
{
	vec3 norm = normalize(vNormal);
	vec3 viewDir = normalize(uViewPos - vWorldPos);

	vec3 diffuseColor = vec3(texture(uMaterial.diffuse, vTexCoord));
	vec3 specularColor = vec3(texture(uMaterial.specular, vTexCoord));

	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);
	for (int i = 0; i < uNumLights; ++i) {
		vec3 lightDir = normalize(uLights[i].position - vWorldPos);

		// ambient light
		ambient += uLights[i].ambient * diffuseColor;

		// diffuse light
		float diff = max(dot(norm, lightDir), 0.0);
		diffuse += uLights[i].diffuse * diff * diffuseColor;

		// specular light
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
		specular += uLights[i].specular * spec * specularColor;
	}

	FragColor = vec4(ambient + diffuse + specular, 1.0);
};