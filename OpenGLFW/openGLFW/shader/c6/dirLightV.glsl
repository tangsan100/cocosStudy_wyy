#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;


uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;
uniform mat4 lightSpaceMat;

void main()
{
    FragPos = vec3(mMatrix * vec4(aPos, 1.0));
    Normal = transpose(inverse(mat3(mMatrix))) * aNormal;
    TexCoords = aTexCoords;
    FragPosLightSpace = lightSpaceMat * vec4(FragPos, 1.0);
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(aPos, 1.0);
}