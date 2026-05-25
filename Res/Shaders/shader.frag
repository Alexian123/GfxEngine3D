#version 330 core

in vec4 vColor;
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vWorldPos;

out vec4 FragColor;

uniform float uBrightness;
uniform sampler2D uTexture;
uniform sampler2D uTexture2;

uniform vec3 uObjectColor;
uniform vec3 uLightColor;
uniform vec3 uLightPos;
uniform vec3 uViewPos;

void main()
{
	//FragColor = mix(texture(uTexture, vTexCoord), texture(uTexture2, vTexCoord), 0.3) * vColor * uBrightness;

	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(uLightPos - vWorldPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * uLightColor;

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * uLightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(uViewPos - vWorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * uLightColor;

	vec3 result = (ambient + diffuse + specular) * uObjectColor;

	FragColor = vec4(result, 1.0);
};