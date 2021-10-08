#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform mat4 mMatrix;

void main()
{
    gl_Position = pMatrix*vMatrix * mMatrix * vec4(aPos, 1.0);
}