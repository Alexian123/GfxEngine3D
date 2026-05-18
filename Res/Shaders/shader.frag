#version 330 core

in vec4 vColor;
in vec2 vTexCoord;

out vec4 FragColor;

uniform float uBrightness;
uniform sampler2D uTexture;
uniform sampler2D uTexture2;

void main()
{
   FragColor = mix(texture(uTexture, vTexCoord), texture(uTexture2, vTexCoord), 0.3) * vColor * uBrightness;
};