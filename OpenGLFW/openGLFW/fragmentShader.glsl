#version 330 core

out vec4 FragColor;
in vec4 ourColor;
in vec2 ourUV;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D ourTexure;

uniform vec3 sunLight;
uniform float ambientStrength;
uniform vec3 lightPos;

uniform vec3 viewPos;

void main()
{
	// ������
	vec3  ambient = sunLight * ambientStrength;

	// ------������ ------
	vec3 normal = normalize(Normal);
	// ��������Ƕ�����
	vec3 lightDir = normalize(lightPos - FragPos);

	// �����ڷ��߷����ϵķ���
	float diff =  max(dot(lightDir,normal),0.0);
	vec3 diffuse = sunLight*diff;


	// ----- ���淴��------
	// ����ǿ��
	float specularStrength = 0.5f;
	// �۲췽��
	vec3 viewDir = normalize(viewPos - FragPos);
	// ���������
	vec3 refectDir = normalize(reflect(-lightDir,normal));
	// ������ڹ۲췽���ͶӰ����32�η�
	float spec = pow(max(dot(viewDir,refectDir),0),128);
	// ���յķ���ǿ�Ƚ��
	vec3 specular = sunLight*spec*specularStrength; 

	// ����ǿ��= ������+������� + ���淴���
	vec3 result = ambient + diffuse + specular;
	FragColor = texture(ourTexure,ourUV) * vec4(result, 1.0f);
};