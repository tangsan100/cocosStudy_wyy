#version 330 core


// ----- 输入输出的变量

out vec4 FragColor;

in vec2 ourUV;
in vec3 FragPos;

uniform sampler2D diffuse;


void main()
{
	FragColor = texture(diffuse,ourUV);
};