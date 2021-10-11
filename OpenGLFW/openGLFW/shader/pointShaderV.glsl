#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // 法线
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent; // 法线贴图

uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform mat4 mMatrix;

out vec2 ourUV;
// out vec3 Normal;
out vec3 FragPos;
out mat3 TBN;

void main()
{
	gl_Position = pMatrix * vMatrix * mMatrix * vec4(aPos,1.0);
	ourUV = aUV;

	FragPos = vec3(mMatrix * vec4(aPos,1.0f));
	// Normal = mat3(transpose(inverse(mMatrix)))*aNormal;

	vec3 N = mat3(transpose(inverse(mMatrix)))*aNormal;
	vec3 T = mat3(transpose(inverse(mMatrix)))*aTangent;
	// 斯密特正交化
	T = normalize(T - dot(T,N)*N);
	vec3 B = cross(N,T);

	TBN = mat3(T,B,N);
};