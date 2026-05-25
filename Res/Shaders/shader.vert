#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec4 vColor;
out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vWorldPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uNormal;

void main()
{
	vec4 worldPos = uModel * vec4(aPos, 1.0);
	gl_Position = uProjection * uView * worldPos;
	vColor = vec4(aColor, 1.0f);
	vTexCoord = aTexCoord;
	vNormal = mat3(uNormal) * aNormal;
	vWorldPos = vec3(worldPos);
}