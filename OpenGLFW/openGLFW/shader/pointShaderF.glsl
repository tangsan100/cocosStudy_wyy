#version 330 core

// ------- 定义材质和光源的结构
// 材质
struct tMaterial{
	sampler2D diffuse1; // 材质贴图
	sampler2D specular1; // 光照贴图

	float shiness;	// 镜面发射衰减次幂
};


// 点光源
struct tLight{
	vec3 pos;

	vec3 ambient; // 环境光
	vec3 diffuse;	// 漫反射系数
	vec3 specular;	// 镜面反射系数

	// 一下3个是计算系数
	float c;
	float l;
	float q;
};



// ----- 输入输出的变量

out vec4 FragColor;
in vec4 ourColor;
in vec2 ourUV;
in vec3 Normal;
in vec3 FragPos;


// 用户传入的参数
uniform tMaterial myMaterial;
uniform tLight  Light;
// uniform sampler2D ourTexure;
uniform vec3 viewPos;

void main()
{
	float dist = length(Light.pos - FragPos);
	float attenuation = 1.0f/(Light.c + Light.l*dist+ Light.q*dist*dist);
	// ---------------环境光------------
	vec3  ambient = Light.ambient * vec3(texture(myMaterial.diffuse1,ourUV));

	// ---------------漫反射 ------
	vec3 normal = normalize(Normal);
	// 阳光照射角度向量
	vec3 lightDir = normalize(Light.pos - FragPos);

	// 阳光在法线方向上的分量
	float diff =  max(dot(lightDir,normal),0.0);
	vec3 diffuse = Light.diffuse*diff*vec3(texture(myMaterial.diffuse1,ourUV));


	// --------------- 镜面反射------
	// 反射强度
	float specularStrength = 0.5f;
	// 观察方向
	vec3 viewDir = normalize(viewPos - FragPos);
	// 反射光向量
	vec3 refectDir = normalize(reflect(-lightDir,normal));
	// 反射光在观察方向的投影，求32次方
	float spec = pow(max(dot(viewDir,refectDir),0),myMaterial.shiness);
	// 最终的反射强度结果
	vec3 specular = Light.specular*spec*vec3(texture(myMaterial.specular1,ourUV)); 

	// ------最终结果
	// 光照强弱= 环境光+漫反射光 + 镜面反射光
	vec3 result = ambient + diffuse + specular;
	// FragColor = texture(myMaterial.specular1,ourUV);
	FragColor = texture(myMaterial.diffuse1,ourUV) * vec4(result, 1.0f)*attenuation;
};