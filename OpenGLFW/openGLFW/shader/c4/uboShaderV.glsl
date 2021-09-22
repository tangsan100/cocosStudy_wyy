#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;


uniform mat4 mMatrix;

layout(std140)uniform MAT{
	mat4 pMatrix;
	mat4 vMatrix;
};

void main()
{
	gl_Position = pMatrix * vMatrix * mMatrix * vec4(aPos.x,aPos.y,aPos.z,1.0);

}