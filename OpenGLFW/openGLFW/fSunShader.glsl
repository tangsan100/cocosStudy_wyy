#version 330 core

out vec4 FragColor;
in vec4 ourColor;
in vec2 ourUV;

uniform sampler2D ourTexure;

void main()
{
	FragColor = vec4(1.0,1.0,1.0,1.0);//texture(ourTexure,ourUV);
};