#version 330 core

out vec4 FragColor;
in vec2 ourUV;

uniform sampler2D cubeCoord;

void main()
{
	FragColor = texture(cubeCoord,ourUV);
};