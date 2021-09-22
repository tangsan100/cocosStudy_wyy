#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;// 法线
layout (location = 2) in vec2 aUV; 
uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform mat4 mMatrix;

out vec2 ourUV;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	gl_Position = pMatrix * vMatrix * mMatrix * vec4(aPos,1.0);
	ourUV = aUV;
};