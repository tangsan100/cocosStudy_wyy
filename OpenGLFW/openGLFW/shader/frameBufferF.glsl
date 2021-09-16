#version 330 core


// ----- 输入输出的变量

out vec4 FragColor;

in vec2 ourUV;

uniform sampler2D diffuse;


void main()
{
	FragColor = texture(diffuse,ourUV);
};