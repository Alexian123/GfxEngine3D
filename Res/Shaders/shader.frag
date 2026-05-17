#version 330 core

in vec4 vColor;
in vec2 vTexCoord;

out vec4 FragColor;

uniform float uBrightness;
uniform sampler2D uTexture;

void main()
{
   FragColor = texture(uTexture, vTexCoord) * uBrightness;
};