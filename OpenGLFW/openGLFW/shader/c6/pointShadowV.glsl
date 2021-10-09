#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform mat4 mMatrix;

uniform bool reverse_normals;

void main()
{
    vs_out.FragPos = vec3(mMatrix * vec4(aPos, 1.0));
    if(reverse_normals) {
        // 是否翻转法线， 大包围盒子需要翻转法线
        vs_out.Normal = transpose(inverse(mat3(mMatrix))) * (-1.0 * aNormal);
    }
    else{
        vs_out.Normal = transpose(inverse(mat3(mMatrix))) * aNormal;
    }
    vs_out.TexCoords = aTexCoords;
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(aPos, 1.0);
}