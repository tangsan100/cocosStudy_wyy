#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 vMatrix;
uniform mat4 pMatrix;

out vec2 ourUV;

void main()
{
	gl_Position = pMatrix * vMatrix * vec4(aPos.x,aPos.y,aPos.z,1.0);
	ourUV = aUV;
};