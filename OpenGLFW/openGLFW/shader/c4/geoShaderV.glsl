#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;// 法线
layout (location = 2) in vec2 aUV; 
uniform mat4 vMatrix;
uniform mat4 mMatrix;

out vec3 normal;

void main()
{
    gl_Position = vMatrix * mMatrix * vec4(aPos,1.0);

    mat3 normalMatrix = mat3(transpose(inverse(vMatrix * mMatrix)));
    normal = vec3(vec4(normalMatrix * aNormal, 0.0));

   
}