#version 330 core


out vec4 FragColor;

uniform sampler2D textureCoord;
in vec2 ourUV;

void main()
{
	FragColor = texture(textureCoord,ourUV);
};