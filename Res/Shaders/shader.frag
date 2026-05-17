#version 330 core

in vec4 vColor;

out vec4 FragColor;

uniform float uBrightness;

void main()
{
   FragColor = vColor * uBrightness;
};