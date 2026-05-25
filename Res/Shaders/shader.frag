#version 330 core

#define MAX_NUM_LIGHTS 10

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec4 vColor;
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vWorldPos;

out vec4 FragColor;

uniform float uBrightness;
uniform sampler2D uTexture;
uniform sampler2D uTexture2;

uniform Material uMaterial;
uniform Light uLights[MAX_NUM_LIGHTS];
uniform vec3 uViewPos;

void main()
{
	//FragColor = mix(texture(uTexture, vTexCoord), texture(uTexture2, vTexCoord), 0.3) * vColor * uBrightness;

	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(uLights[0].position - vWorldPos);

	// ambient light
	vec3 ambient = uLights[0].ambient * uMaterial.ambient;

	// diffuse light
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = uLights[0].diffuse * (diff * uMaterial.diffuse);

	// specular light
	vec3 viewDir = normalize(uViewPos - vWorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
	vec3 specular = uLights[0].specular * (spec * uMaterial.specular);

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
};