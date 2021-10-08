#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 vMatrix;
uniform mat4 pMatrix;


out vec3 Normal;
out vec2 ourUV;

void main()
{
	
	vec4 pos = pMatrix * vMatrix * vec4(aPos,1.0);

	gl_Position = pos.xyzw;
	ourUV = aUV;
};